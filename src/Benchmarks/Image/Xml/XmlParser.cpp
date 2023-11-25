//
// Created by klapeto on 22/11/2023.
//

#include "XmlParser.hpp"
#include "CharacterStream.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "XmlElement.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace Elpida
{
	enum class NextNode
	{
		Element,
		Content,
		Cdata
	};

	static NextNode SkipUnusableAndGetNextNodeType(CharacterStream& stream)
	{
		while (!stream.Eof())
		{
			stream.SkipSpace();
			auto c = stream.Current();
			if (c == '<')
			{
				if (stream.Next())
				{
					c = stream.Current();
					if (c == '?')
					{
						if (!stream.SkipUntilString("?>"))
						{
							throw ElpidaException("Unexpected end of file: expected '?>'");
						}
					}
					else if (c == '!')
					{
						if (!stream.Next())
						{
							throw ElpidaException("Unexpected end of file: expected '-' or '[CDATA['");
						}
						if (stream.Current() == '-')
						{
							if (!stream.Next())
							{
								throw ElpidaException("Unexpected end of file: expected '-'");
							}

							if (stream.Current() != '-')
							{
								throw ElpidaException("Unexpected character: expected '-'");
							}

							if (!stream.SkipUntilString("-->"))
							{
								throw ElpidaException("Unexpected end of file: expected '-->'");
							}
						}
						else if (stream.Current() == '[')
						{
							if (!stream.NextCharsAre("[CDATA["))
							{
								throw ElpidaException("Unexpected character: expected '[CDATA['");
							}
							stream.Next();
							return NextNode::Cdata;
						}
						else
						{
							throw ElpidaException("Unexpected character: expected '-' or '[CDATA['");
						}
					}
					else
					{
						return NextNode::Element;
					}
				}
				else
				{
					throw ElpidaException("Unexpected end of file: ended with <");
				}
			}
			else if (c == '>')
			{
				stream.Next();
				continue;
			}
			else
			{
				return NextNode::Content;
			}
			stream.Next();
		}
		return NextNode::Content;
	}

	static std::unordered_map<std::string, std::string> ParseAttributes(CharacterStream& stream, bool& inlineElement)
	{
		std::unordered_map<std::string, std::string> returnAttributes;

		while (!stream.Eof())
		{
			stream.SkipSpace();
			if (stream.Current() == '/')
			{
				if (!stream.Next())
				{
					throw ElpidaException("Unexpected end of file. Expected />");
				}
				if (stream.Current() == '>')
				{
					inlineElement = true;
					stream.Next();
					break;
				}

				throw ElpidaException("Unexpected end of file. Expected '>' after '/'");
			}
			if (stream.Current() == '>')
			{
				inlineElement = false;
				stream.Next();
				break;
			}

			auto name = stream.GetStringViewWhile([](auto c) { return c != '='; });
			if (!stream.Next())
			{
				throw ElpidaException("Unexpected end of file. Expected attribute='value'");
			}

			auto quote = stream.Current();
			if (quote != '\"' && quote != '\'')
			{
				throw ElpidaException("Unexpected character after =. Expected quote ' or \"");
			}

			if (!stream.Next())
			{
				throw ElpidaException("Unexpected end of file. Expected attribute='value'");
			}

			auto value = stream.GetStringViewWhile([quote](auto c) { return c != quote; });

			returnAttributes.insert({ std::string(name), std::string(value) });
			stream.Next();
		}

		return returnAttributes;
	}

	static std::string ParseName(CharacterStream& stream)
	{
		if (CharacterStream::isspace(stream.Current()))
		{
			throw ElpidaException("Unexpected space after '<'");
		}
		return std::string(stream.GetStringViewWhile([](auto c) { return !CharacterStream::isspace(c) && c != '>'; }));
	}

	static XmlElement ParseElement(CharacterStream& stream)
	{
		if (stream.Eof())
		{
			throw ElpidaException("Unexpected end of file. Expected <element>");
		}
		auto name = ParseName(stream);

		bool inlineElement;
		auto attributes = ParseAttributes(stream, inlineElement);

		if (inlineElement)
		{
			return {std::move(name), std::move(attributes), {}, {}};
		}

		std::vector<XmlElement> children;

		while (!stream.Eof())
		{
			auto type = SkipUnusableAndGetNextNodeType(stream);
			if (type == NextNode::Element)
			{
				if (stream.Current() == '/')
				{
					stream.Next();
					auto currentName = stream.GetStringViewWhile([](auto c) { return c != '>'; });
					if (currentName != name)
					{
						throw ElpidaException("Unexpected end of element name");
					}
					break;
				}
				children.push_back(ParseElement(stream));
			}
			else if (type == NextNode::Cdata)
			{
				auto firstCharacter = stream.Index();

				if (!stream.SkipUntilString("]]>"))
				{
					throw ElpidaException("Unexpected end of file: expected ']]'");
				}

				auto content = std::string(stream.GetStringView(firstCharacter, stream.Index() - 2));
				children.push_back(XmlElement("", {}, std::move(content), {}));
			}
			else
			{
				auto firstCharacter = stream.Index();
				auto lastCharacter = firstCharacter;
				while (!stream.Eof())
				{
					if (stream.Current() == '<')
					{
						break;
					}
					if (!CharacterStream::isspace(stream.Current()))
					{
						lastCharacter = stream.Index();
					}
					stream.Next();
				}
				auto content = std::string(stream.GetStringView(firstCharacter, lastCharacter + 1));
				children.push_back(XmlElement("", {}, std::move(content), {}));
			}
		}

		return {std::move(name), std::move(attributes), {}, std::move(children)};
	}

	XmlElement XmlParser::Parse(const char* data, std::size_t size)
	{
		CharacterStream stream(data, size);

		if (SkipUnusableAndGetNextNodeType(stream) != NextNode::Element)
		{
			throw ElpidaException("Unexpected end of file: expected at least one <element>");
		}
		return ParseElement(stream);
	}
} // Elpida