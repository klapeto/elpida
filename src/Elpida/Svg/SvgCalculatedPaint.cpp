//
// Created by klapeto on 27/1/2024.
//

#include "Elpida/Svg/SvgCalculatedPaint.hpp"

#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgPaint.hpp"
#include "Elpida/Svg/SvgBounds.hpp"
#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"

namespace Elpida
{
	SvgColor SvgCalculatedPaint::CalculateColor(const SvgPoint& point) const
	{
		if (_color)
		{
			return std::get<SvgColor>(_state);
		}

		// TODO: Create a delegate: Measure impact
		if (_gradientType == SvgGradientType::Linear)
		{
			return std::get<SvgCalculatedLinearGradient>(_state).CalculateColor(point);
		}
		else
		{
			return std::get<SvgCalculatedRadialGradient>(_state).CalculateColor(point);
		}
	}

	SvgCalculatedPaint::SvgCalculatedPaint(const SvgPaint& paint,
			const SvgBounds& elementBounds,
			const SvgDocument& document,
			const SvgCalculationContext& calculationContext)
			:_gradientType(SvgGradientType::Linear), _color(true)
	{
		if (paint.GetGradientId().empty())
		{
			AsColor(paint);
		}
		else
		{
			AsGradient(paint, elementBounds, document, calculationContext);
		}
		_opacity = paint.GetOpacity();
	}

	void SvgCalculatedPaint::AsGradient(const SvgPaint& paint,
			const SvgBounds& elementBounds,
			const SvgDocument& document,
			const SvgCalculationContext& calculationContext)
	{
		const SvgGradient* gradient;
		const SvgGradient* stopsGradient;
		GetGradients(document, paint, &gradient, &stopsGradient);

		if (gradient == nullptr || stopsGradient == nullptr) return;

		_color = false;
		_gradientType = gradient->GetType();

		if (_gradientType == SvgGradientType::Linear)
		{
			_state = SvgCalculatedLinearGradient(*gradient, elementBounds,
					CalculateStops(*stopsGradient, calculationContext), calculationContext);
		}
		else
		{
			_state = SvgCalculatedRadialGradient(*gradient, elementBounds,
					CalculateStops(*stopsGradient, calculationContext), calculationContext);
		}
	}

	void SvgCalculatedPaint::AsColor(const SvgPaint& paint)
	{
		_state = paint.GetColor();
	}

	std::vector<SvgCalculatedGradientStop> SvgCalculatedPaint::CalculateStops(const SvgGradient& gradientStops,
			const SvgCalculationContext& calculationContext)
	{
		std::vector<SvgCalculatedGradientStop> calculatedStops;
		calculatedStops.reserve(gradientStops.GetStops().size());

		for (auto& stop : gradientStops.GetStops())
		{
			calculatedStops.emplace_back(stop.GetColor(),
					stop.GetOffset().CalculateValue(calculationContext, 1.0),
					stop.GetOpacity().CalculateValue(calculationContext, 1.0));
		}

		return calculatedStops;
	}

	void SvgCalculatedPaint::GetGradients(const SvgDocument& document, const SvgPaint& paint,
			const SvgGradient** gradient, const SvgGradient** gradientStops)
	{
		const auto gradientId = paint.GetGradientId().substr(1); //ignore the '#'
		const auto gradientItr = document.GetDefs().find(gradientId);

		*gradientStops = nullptr;
		*gradient = nullptr;

		if (gradientItr != document.GetDefs().end())
		{
			*gradient = &gradientItr->second;

			auto gradientStopsItr = gradientItr;
			if ((*gradient)->GetStops().empty())
			{
				auto& ref = (*gradient)->GetHref();
				if (!ref.empty())
				{
					gradientStopsItr = document.GetDefs().find(ref.substr(1));
					if (gradientStopsItr == document.GetDefs().end())
					{
						return;
					}
				}
			}
			*gradientStops = &gradientStopsItr->second;
		}
	}

	SvgCalculatedPaint::SvgCalculatedPaint()
			:_gradientType(SvgGradientType::Linear), _color(true), _opacity(1.0)
	{

	}

	void SvgCalculatedPaint::Transform(const SvgTransform& transform)
	{
		if (_color) return;

		if (_gradientType == SvgGradientType::Linear)
		{
			std::get<SvgCalculatedLinearGradient>(_state).Transform(transform);
		}
		else
		{
			std::get<SvgCalculatedRadialGradient>(_state).Transform(transform);
		}
	}
} // Elpida
