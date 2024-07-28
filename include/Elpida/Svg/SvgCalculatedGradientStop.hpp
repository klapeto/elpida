//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDGRADIENTSTOP_HPP
#define ELPIDA_SVGCALCULATEDGRADIENTSTOP_HPP

#include "SvgColor.hpp"

namespace Elpida
{
	class SvgCalculatedGradientStop
	{
	public:
		[[nodiscard]]
		const SvgColor& GetColor() const
		{
			return _color;
		}

		[[nodiscard]]
		double GetOffset() const
		{
			return _offset;
		}

		[[nodiscard]]
		double GetOpacity() const
		{
			return _opacity;
		}

		SvgCalculatedGradientStop(const SvgColor& color, double offset, double opacity);
	private:
		SvgColor _color;
		double _offset;
		double _opacity;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDGRADIENTSTOP_HPP
