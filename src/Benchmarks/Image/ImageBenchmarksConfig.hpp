//
// Copyright (C) 2025. Ioannis Panagiotopoulos
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 5/3/2023.
//

#ifndef _IMAGEBENCHMARKSCONFIG_HPP_
#define _IMAGEBENCHMARKSCONFIG_HPP_

#include "Elpida/Core/Float.hpp"
#include <cstdint>

namespace Elpida
{
	using FloatChannel = Float;
	using IntChannel = uint8_t;

	constexpr const char* WidthProperty = "width";
	constexpr const char* HeightProperty = "height";
	constexpr const char* BytesPerChannelProperty = "bytesPerChannel";
	constexpr const char* ChannelsProperty = "channels";
}

#endif //_IMAGEBENCHMARKSCONFIG_HPP_
