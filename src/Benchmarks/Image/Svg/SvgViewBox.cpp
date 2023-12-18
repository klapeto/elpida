//
// Created by klapeto on 11/12/2023.
//

#include "SvgViewBox.hpp"

#include <Benchmarks/Image/Xml/CharacterStream.hpp>

#include "SvgNumber.hpp"

namespace Elpida
{
	SvgViewBox::SvgViewBox(std::string_view view)
	{
		if (view.empty())
		{
			_width = 0;
			_height = 0;
			_minX = 0;
			_minY = 0;
			return;
		}

		CharacterStream stream(view);

		auto callback = [](auto c) { return CharacterStream::IsSpace(c) || c == ',' || c == '%'; };

		_minX = SvgNumber::ParseNumber(stream);
		stream.Skip(callback);

		if (stream.Eof())
		{
			throw ElpidaException("Unexpected Eof: expected view box 'minX minY width height'");
		}

		_minY = SvgNumber::ParseNumber(stream);
		stream.Skip(callback);

		if (stream.Eof())
		{
			throw ElpidaException("Unexpected Eof: expected view box 'minX minY width height'");
		}

		_width = SvgNumber::ParseNumber(stream);
		stream.Skip(callback);

		if (stream.Eof())
		{
			throw ElpidaException("Unexpected Eof: expected view box 'minX minY width height'");
		}

		_height = SvgNumber::ParseNumber(stream);
	}
} // Elpida