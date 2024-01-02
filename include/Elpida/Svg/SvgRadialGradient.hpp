//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGRADIALGRADIENT_HPP
#define ELPIDA_SVG_SVGRADIALGRADIENT_HPP

#include "SvgGradient.hpp"

namespace Elpida
{
	class SvgRadialGradient: public SvgGradient
	{
	public:
		[[nodiscard]] const SvgLength& GetCx() const
		{
			return _cx;
		}

		[[nodiscard]] const SvgLength& GetCy() const
		{
			return _cy;
		}

		[[nodiscard]] const SvgLength& GetR() const
		{
			return _r;
		}

		[[nodiscard]] const SvgLength& GetFx() const
		{
			return _fx;
		}

		[[nodiscard]] const SvgLength& GetFy() const
		{
			return _fy;
		}

		explicit SvgRadialGradient(const XmlElement& element, SvgDocument& document);
	private:
		SvgLength _cx;
		SvgLength _cy;
		SvgLength _r;
		SvgLength _fx;
		SvgLength _fy;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGRADIALGRADIENT_HPP
