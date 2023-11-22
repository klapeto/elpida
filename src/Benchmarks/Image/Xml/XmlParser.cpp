//
// Created by klapeto on 22/11/2023.
//

#include "XmlParser.hpp"
#include "CharacterStream.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "XmlElement.hpp"
#include <cctype>
#include <string>
#include <unordered_map>

namespace Elpida
{

	static void SkipUnusable(CharacterStream& stream)
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
					if (c == '?' || c == '!')
					{
						stream.Skip([](auto c) { return c != '>'; });
					}
					else
					{
						break;
					}
				}
				else
				{
					throw ElpidaException("Unexpected end of file: ended with <");
				}
			}
			else if (c == '>')
			{
				continue;
			}
			else
			{
				throw ElpidaException("Unexpected character: expected <");
			}
			stream.Next();
		}
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
					break;
				}

				throw ElpidaException("Unexpected end of file. Expected '>' after '/'");
			}
			if (stream.Current() == '>')
			{
				inlineElement = false;
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
		stream.SkipSpace();
		if (stream.Current() == '<')
		{
			stream.Next();
		}
		if (std::isspace(stream.Current()))
		{
			throw ElpidaException("Unexpected space after '<'");
		}
		return std::string(stream.GetStringViewWhile([](auto c) { return !std::isspace(c); }));
	}

	static XmlElement ParseElement(CharacterStream& stream)
	{
		SkipUnusable(stream);

		if (stream.Eof())
		{
			throw ElpidaException("Unexpected end of file. Expected at least one <element>");
		}
		auto name = ParseName(stream);

		bool inlineElement;
		auto attributes = ParseAttributes(stream, inlineElement);

		return XmlElement();
	}

	XmlElement XmlParser::Parse(const char* data, std::size_t size)
	{
		CharacterStream stream(data, size);

		return ParseElement(stream);
	}
} // Elpida