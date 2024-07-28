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

	void SvgCalculatedStroke::Transform(const SvgTransform& transform)
	{
		SvgCalculatedPaint::Transform(transform);

		auto value= (transform[SvgTransform::A] + transform[SvgTransform::D]) / 2.0;
		_width *=  value;

		for (double & dash : _dashes)
		{
			dash *= value;
		}

		_dashOffset *= value;
	}

} // Elpida