//
// Created by klapeto on 9/3/2024.
//

#include "Elpida/Svg/SvgCalculatedStroke.hpp"

#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgStroke.hpp"

namespace Elpida
{
	SvgCalculatedStroke::SvgCalculatedStroke(const SvgStroke& stroke, const SvgBounds& elementBounds,
			const SvgDocument& document, const SvgCalculationContext& calculatingContext)
			:SvgCalculatedPaint(stroke, elementBounds, document, calculatingContext)
	{
		auto length = calculatingContext.GetViewBox().GetLength();

		for (auto& dash : stroke.GetDashes())
		{
			_dashes.push_back(dash.CalculateValue(calculatingContext, length));
		}

		_width = stroke.GetWidth().CalculateValue(calculatingContext, length);
		_dashOffset = stroke.GetDashOffset().CalculateValue(calculatingContext, length);
		_miterLimit = stroke.GetMiterLimit();
		_lineCap = stroke.GetLineCap();
		_lineJoin = stroke.GetLineJoin();
	}

	const std::vector<double>& SvgCalculatedStroke::GetDashes() const
	{
		return _dashes;
	}

	SvgLineJoin SvgCalculatedStroke::GetLineJoin() const
	{
		return _lineJoin;
	}

	SvgLineCap SvgCalculatedStroke::GetLineCap() const
	{
		return _lineCap;
	}

	double SvgCalculatedStroke::GetDashOffset() const
	{
		return _dashOffset;
	}

	double SvgCalculatedStroke::GetWidth() const
	{
		return _width;
	}

	double SvgCalculatedStroke::GetMiterLimit() const
	{
		return _miterLimit;
	}

} // Elpida