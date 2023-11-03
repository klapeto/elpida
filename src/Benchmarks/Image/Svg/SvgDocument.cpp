//
// Created by klapeto on 3/11/2023.
//

#include "SvgDocument.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#define ERROR(x) throw Elpida::ElpidaException(x)
#define CHECK() (i < size)
#define CHECK_E(x) if (!CHECK()) throw Elpida::ElpidaException(x)

#define STRING_VIEW() { &data[currentIndex], i - currentIndex }

namespace Elpida
{

	enum class State
	{
		CONTENT,
		TAG
	};

	void SvgDocument::Load(const char* data, std::size_t size, SvgUnits units, float dpi)
	{
		State state = State::CONTENT;
		for (std::size_t i = 0; i < size; ++i)
		{
			auto c = data[i];
			switch (c)
			{
			case '<':
				ParseElement(data, i, size);
			case '>':
			default:
				continue;
			}
		}
	}

	static std::string_view GetElementName(const char* data, std::size_t& i, std::size_t size)
	{
		auto currentIndex = i;
		while (i < size && !std::isspace(data[i])) i++;

		return STRING_VIEW();
	}

	static void GetAttribute(const char* data,
		std::size_t& i,
		std::size_t size,
		std::string_view& name,
		std::string_view& value)
	{
		auto currentIndex = i;

		// Go until space or =
		while (CHECK() && !std::isspace(data[i]) && data[i] != '=') i++;

		CHECK_E("Unexpected end of file: Element was not closed");

		name = STRING_VIEW();

		// Skip space
		while (CHECK() && std::isspace(data[i])) i++;

		CHECK_E("Unexpected end of file: Element was not closed");

		if (data[i] == '=')
		{
			i++;
		}
		else
		{
			// no value
			return;
		}

		// Skip space until quote
		while (CHECK())
		{
			auto c = data[i];
			if (std::isspace(data[i]))
			{
				continue;
			}

			if (c == '\"' || c == '\'')
			{
				break;
			}
			else
			{
				ERROR("Unexpected character: expected start quote");
			}
		}

		CHECK_E("Unexpected end of file: expected start quote");

		char quote = data[i++];
		currentIndex = i;

		while (CHECK() && data[i] != quote) i++;

		CHECK_E("Unexpected end of file: expected end quote");

		if (data[i] != quote)
		{
			ERROR("Unexpected end of file: expected end quote");
		}
		value = STRING_VIEW();
	}

	void SvgDocument::ParseElement(const char* data, std::size_t& i, std::size_t size)
	{
		// Skip '<'
		if (++i >= size) ERROR("Unexpected end of file: '<'");

		// Skip comments and declarations
		if (data[i] == '!' || data[i] == '?')
		{
			while (i < size && data[i] != '>') i++;
			if (i < size) i++;
			return;
		}

		std::string_view elementName = GetElementName(data, i, size);

		while (i < size && data[i] != '>')
		{
			while (i < size && std::isspace(data[i])) i++;
			std::string_view name, value;
			GetAttribute(data, i,size, name, value);

			if (value.empty()) continue;


		}
	}

} // Elpida