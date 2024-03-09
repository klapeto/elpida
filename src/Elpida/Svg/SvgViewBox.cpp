//
// Created by klapeto on 11/12/2023.
//

#include "Elpida/Svg/SvgViewBox.hpp"

#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Xml/CharacterStream.hpp"

namespace Elpida
{
	SvgViewBox::SvgViewBox(std::string_view view)
	{
		if (view.empty())
		{
			_width = 0;
			_height = 0;
			return;
		}

		CharacterStream stream(view);

		auto callback = [](auto c) { return CharacterStream::IsSpace(c) || c == ',' || c == '%'; };

		auto minX = SvgNumber::ParseNumber(stream);
		stream.Skip(callback);

		if (stream.Eof())
		{
			throw ParseException("Unexpected Eof: expected view box 'minX minY width height'");
		}

		auto minY = SvgNumber::ParseNumber(stream);

		_min = SvgPoint(minX, minY);

		stream.Skip(callback);

		if (stream.Eof())
		{
			throw ParseException("Unexpected Eof: expected view box 'minX minY width height'");
		}

		_width = SvgNumber::ParseNumber(stream);
		stream.Skip(callback);

		if (stream.Eof())
		{
			throw ParseException("Unexpected Eof: expected view box 'minX minY width height'");
		}

		_height = SvgNumber::ParseNumber(stream);

		CalculateLength();
	}

	static constexpr double Sqrt(const double x)
	{
		return std::sqrt(x);
	}

	void SvgViewBox::CalculateLength()
	{
		_length =  Sqrt(_width * _width + _height * _height) / Sqrt(2.0);
	}

	SvgViewBox::SvgViewBox(const double minX, const double minY, const double width, const double height)
			:_min(minX, minY),
			 _width(width),
			 _height(height),
			 _length(0.0)
	{
		CalculateLength();
	}

	SvgViewBox::SvgViewBox()
			:_width(0.0),
			 _height(0.0),
			 _length(0.0)
	{
	}
} // Elpida