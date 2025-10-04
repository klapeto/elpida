/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgFontSize.hpp"

#include <functional>
#include <unordered_map>
#include <Elpida/Svg/SvgLength.hpp>
#include <Elpida/Xml/CharacterStream.hpp>

namespace Elpida
{
	SvgFontSize::SvgFontSize(const std::string_view view)
	{
		_length = SvgLength(view);
		if (_length.GetValue() <= 0.0)
		{
			static std::unordered_map<std::string_view, SvgFloat> transformers
			{
				{"xx-small", 3.0 / 5.0},
				{"x-small ", 3.0 / 4.0},
				{"small ", 8.0 / 9.0},
				{"medium ", 1.0},
				{"large ", 6.0 / 5.0},
				{"x-large ", 3.0 / 2.0},
				{"xx-large ", 2.0 / 1.0},
				{"xxx-large ", 3.0 / 1.0},
			};
			CharacterStream stream(view);
			stream.SkipSpace();
			const auto value = stream.GetStringViewWhile([](auto c) { return !CharacterStream::IsSpace(c); });

			if (const auto itr = transformers.find(value); itr != transformers.end())
			{
				_length = SvgLength(itr->second, SvgUnits::Raw);
			}
			else
			{
				if (value == "smaller")
				{
					_length = SvgLength(0.8, SvgUnits::Em);
				}
				else if (value == "larger")
				{
					_length = SvgLength(1.2, SvgUnits::Em);
				} else
				{
					_length = SvgLength(1.0, SvgUnits::Raw);
				}
			}
		}
	}
} // Elpida
