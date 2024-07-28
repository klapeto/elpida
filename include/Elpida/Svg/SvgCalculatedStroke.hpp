//
// Created by klapeto on 9/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDSTROKE_HPP
#define ELPIDA_SVGCALCULATEDSTROKE_HPP

#include <vector>
#include "Elpida/Svg/SvgLineCap.hpp"
#include "Elpida/Svg/SvgLineJoin.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"

namespace Elpida
{

	class SvgStroke;
	class SvgCalculationContext;

	class SvgCalculatedStroke : public SvgCalculatedPaint
	{
	public:
		[[nodiscard]]
		const std::vector<double>& GetDashes() const
		{
			return _dashes;
		}

		[[nodiscard]]
		SvgLineJoin GetLineJoin() const
		{
			return _lineJoin;
		}

		[[nodiscard]]
		SvgLineCap GetLineCap() const
		{
			return _lineCap;
		}

		[[nodiscard]]
		double GetDashOffset() const
		{
			return _dashOffset;
		}

		[[nodiscard]]
		double GetWidth() const
		{
			return _width;
		}

		[[nodiscard]]
		double GetMiterLimit() const
		{
			return _miterLimit;
		}

		void Transform(const SvgTransform& transform) override;

		SvgCalculatedStroke(const SvgStroke& stroke, const SvgBounds& elementBounds, const SvgDocument& document,
				const SvgCalculationContext& calculatingContext);
		SvgCalculatedStroke(const SvgCalculatedStroke&) = default;
		SvgCalculatedStroke& operator=(const SvgCalculatedStroke&) = default;
		SvgCalculatedStroke(SvgCalculatedStroke&&) noexcept = default;
		SvgCalculatedStroke& operator=(SvgCalculatedStroke&&) noexcept = default;
		~SvgCalculatedStroke() override = default;
	private:
		std::vector<double> _dashes;
		SvgLineJoin _lineJoin;
		SvgLineCap _lineCap;
		double _dashOffset;
		double _width;
		double _miterLimit;
	};

} // Svg

#endif //ELPIDA_SVGCALCULATEDSTROKE_HPP
