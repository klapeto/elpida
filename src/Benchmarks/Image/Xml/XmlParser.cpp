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
							if (!stream.ConsumeNextChars("[CDATA["))
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

	static XmlElement::Attributes ParseAttributes(CharacterStream& stream, bool& inlineElement)
	{
		XmlElement::Attributes returnAttributes;

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
					break;
				}

				throw ElpidaException("Unexpected end of file. Expected '>' after '/'");
			}
			if (stream.Current() == '>')
			{
				inlineElement = false;
				break;
			}

			auto name = stream.GetStringViewWhile([](auto c) { return !CharacterStream::IsSpace(c) && c != '='; });

			stream.Skip([](auto c) { return CharacterStream::IsSpace(c) || c == '='; });

			auto quote = stream.Current();
			if (quote != '\"' && quote != '\'')
			{
				throw ElpidaException(std::string("Unexpected character after = (") + quote + "). Expected quote ' or \"");
			}

			if (!stream.Next())
			{
				throw ElpidaException("Unexpected end of file. Expected attribute='value'");
			}

			auto value = stream.GetStringViewWhile([quote](auto c) { return c != quote; });

			returnAttributes.insert({XmlElement::String(name), XmlElement::String(value)});
			stream.Next();
		}

		if (stream.Current() != '>')
		{
			throw ElpidaException("Unexpected character. Expected end of element '>");
		}

		return returnAttributes;
	}

	static XmlElement::String ParseName(CharacterStream& stream)
	{
		if (CharacterStream::IsSpace(stream.Current()))
		{
			throw ElpidaException("Unexpected space after '<'");
		}
		return XmlElement::String(stream.GetStringViewWhile([](auto c)
		{
			return !CharacterStream::IsSpace(c) && c != '>';
		}));
	}

	static XmlElement ParseElement(CharacterStream& stream)
	{
		if (stream.Eof())
		{
			throw ElpidaException("Unexpected end of file. Expected <element>");
		}
		auto name = ParseName(stream);

		if (name.empty())
		{
			throw ElpidaException("Unexpected empty name of element. Expected <element>");
		}
		bool inlineElement;
		auto attributes = ParseAttributes(stream, inlineElement);

		if (inlineElement)
		{
			return {std::move(name), std::move(attributes), {}, {}};
		}

		XmlElement::Children children;

		while (!stream.Eof())
		{
			if (auto type = SkipUnusableAndGetNextNodeType(stream); type == NextNode::Element)
			{
				if (stream.Current() == '/')
				{
					stream.Next();
					if (stream.GetStringViewWhile([](auto c) { return c != '>'; }) != name)
					{
						throw ElpidaException("Unexpected end of element name");
					}
					return {std::move(name), std::move(attributes), {}, std::move(children)};
				}
				children.push_back(ParseElement(stream));
			}
			else if (type == NextNode::Cdata)
			{
				std::ostringstream buffer;
				bool spaceNeeded = false;
				stream.SkipSpace();
				while (!stream.Eof())
				{
					if (const auto c = stream.Current(); c == ']')
					{
						if (stream.ConsumeNextCharsCond("]]>"))
						{
							break;
						}
					}
					else if (CharacterStream::IsSpace(c))
					{
						spaceNeeded = true;
					}
					else
					{
						if (spaceNeeded)
						{
							buffer << ' ';
							spaceNeeded = false;
						}
						buffer << c;
					}
					stream.Next();
				}
				children.push_back(XmlElement("", {}, buffer.str(), {}));
			}
			else
			{
				std::ostringstream buffer;
				bool spaceNeeded = false;
				stream.SkipSpace();
				while (!stream.Eof())
				{
					if (const auto c = stream.Current(); c == '<')
					{
						break;
					}
					else if (CharacterStream::IsSpace(c))
					{
						spaceNeeded = true;
					}
					else
					{
						if (spaceNeeded)
						{
							buffer << ' ';
							spaceNeeded = false;
						}
						buffer << c;
					}
					stream.Next();
				}
				children.push_back(XmlElement("", {}, buffer.str(), {}));
			}
		}

		throw ElpidaException("Unexpected end of element name");
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
