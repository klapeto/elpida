//
// Created by klapeto on 9/3/2024.
//

#ifndef ELPIDA_SVGVIEWPORT_HPP
#define ELPIDA_SVGVIEWPORT_HPP

#include "SvgLength.hpp"

namespace Elpida
{
	class XmlMap;

	class SvgViewPort
	{
	public:
		[[nodiscard]]
		const SvgLength& GetX() const;

		[[nodiscard]]
		const SvgLength& GetY() const;

		[[nodiscard]]
		const SvgLength& GetWidth() const;

		[[nodiscard]]
		const SvgLength& GetHeight() const;

		SvgViewPort();
		explicit SvgViewPort(const XmlMap& properties);
	private:
		SvgLength _x;
		SvgLength _y;
		SvgLength _width;
		SvgLength _height;
	};

} // Elpida

#endif //ELPIDA_SVGVIEWPORT_HPP
