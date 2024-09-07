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
		SvgFloat GetOffset() const
		{
			return _offset;
		}

		[[nodiscard]]
		SvgFloat GetOpacity() const
		{
			return _opacity;
		}

		SvgCalculatedGradientStop(const SvgColor& color, SvgFloat offset, SvgFloat opacity);
	private:
		SvgColor _color;
		SvgFloat _offset;
		SvgFloat _opacity;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDGRADIENTSTOP_HPP
