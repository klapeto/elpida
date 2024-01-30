//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOLOR_HPP
#define ELPIDA_SVG_SVGCOLOR_HPP

#include <string_view>

namespace Elpida
{
	class SvgColor
	{
	public:
		[[nodiscard]]
		unsigned char R() const
		{
			return _r;
		}

		[[nodiscard]]
		unsigned char G() const
		{
			return _g;
		}

		[[nodiscard]]
		unsigned char B() const
		{
			return _b;
		}

		[[nodiscard]]
		unsigned char A() const
		{
			return _a;
		}

		SvgColor()
			: _r(0), _g(0), _b(0), _a(255)
		{
		}

		SvgColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
			: _r(r),
			  _g(g),
			  _b(b),
			  _a(a)
		{
		}

		explicit SvgColor(std::string_view view);

	private:
		unsigned char _r;
		unsigned char _g;
		unsigned char _b;
		unsigned char _a;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGCOLOR_HPP
