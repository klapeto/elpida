//
// Created by klapeto on 27/1/2024.
//

#include "Elpida/Svg/SvgRasterizerPaint.hpp"

#include <Elpida/Svg/SvgDocument.hpp>

namespace Elpida
{
	SvgColor SvgRasterizerPaint::CalculateColor(const SvgPoint& point) const
	{
		if (_gradient == nullptr || _stopsGradient == nullptr)
		{
			return _color;
		}

		if (_gradient->GetType() == SvgGradientType::Linear)
		{
			std::vector<SvgPoint> stopPoints;
			stopPoints.reserve(_gradient->GetStops().size());

			for (auto& stop: _gradient->GetStops())
			{
				stop.
			}
		}
	}

	SvgRasterizerPaint::SvgRasterizerPaint(const SvgPaint& paint, const SvgDocument& document)
		: _gradient(nullptr), _stopsGradient(nullptr)
	{
		if (paint.GetGradientId().empty())
		{
			AsColor(paint);
		}
		else
		{
			AsGradient(paint, document);
		}
	}

	void SvgRasterizerPaint::AsGradient(const SvgPaint& paint, const SvgDocument& document)
	{
		const auto gradientId = paint.GetGradientId().substr(1); //ignore the '#'
		const auto gradientItr = document.GetDefs().find(gradientId);

		if (gradientItr != document.GetDefs().end())
		{
			_gradient = &gradientItr->second;

			auto gradientStops = gradientItr;
			if (_gradient->GetStops().empty())
			{
				auto& ref = _gradient->GetHref();
				if (!ref.empty())
				{
					gradientStops = document.GetDefs().find(ref.substr(1));
					if (gradientStops == document.GetDefs().end())
					{
						return;
					}
				}
			}

			_stopsGradient = &gradientStops->second;
		}
	}

	void SvgRasterizerPaint::AsColor(const SvgPaint& paint)
	{
		_color = paint.GetColor();
	}
} // Elpida
