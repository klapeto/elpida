//
// Created by klapeto on 5/3/2023.
//

#ifndef _IMAGEBENCHMARKSCONFIG_HPP_
#define _IMAGEBENCHMARKSCONFIG_HPP_

#include <cstdint>

namespace Elpida
{
	using FloatChannel = float;
	using IntChannel = uint8_t;

	constexpr const char* WidthProperty = "width";
	constexpr const char* HeightProperty = "height";
	constexpr const char* BytesPerChannelProperty = "bytesPerChannel";
	constexpr const char* ChannelsProperty = "channels";
}

#endif //_IMAGEBENCHMARKSCONFIG_HPP_
