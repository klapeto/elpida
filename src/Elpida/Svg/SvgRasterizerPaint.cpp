//
// Created by klapeto on 27/1/2024.
//

#include "Elpida/Svg/SvgRasterizerPaint.hpp"

#include <Elpida/Svg/SvgDocument.hpp>

#include "Elpida/Svg/SvgLinearEquation.hpp"

namespace Elpida
{
	SvgColor SvgRasterizerPaint::CalculateColor(const SvgPoint& point, const SvgDocument& document) const
	{
		if (_gradient == nullptr || _stopsGradient == nullptr)
		{
			return _color;
		}

		if (_gradient->GetType() == SvgGradientType::Linear)
		{
			auto& data = _gradient->GetData().linear;

			auto x1 = data.x1.CalculateActualValue(document, 0.0, 1.0);

			auto a = SvgPoint(
				x1,
				data.y1.CalculateActualValue(document, 0.0, 1.0)
			);

			auto b = SvgPoint(
				data.x2.CalculateActualValue(document, 0.0, 1.0),
				data.y2.CalculateActualValue(document, 0.0, 1.0)
			);

			const SvgLinearEquation equation(a, b);

			auto distance = a.GetDistance(b);

			const SvgGradientStop* stopA = nullptr;
			const SvgGradientStop* stopB = nullptr;
			SvgPoint pointA = a;
			SvgPoint pointB = b;
			for (auto& stop : _stopsGradient->GetStops())
			{
				if (stopA == nullptr)
				{
					stopA = &stop;
					stopB = &stop;
					continue;
				}

				auto x = x1 + (x1 * stop.GetOffset());
				auto y = equation.CalculateY(x);

				pointB = SvgPoint(x, y);
				SvgLinearEquation thisEquation(pointA, pointB);

				if (!thisEquation.GetANormal().IsLeftOf(point) && thisEquation.GetBNormal().IsLeftOf(point))
				{
					stopA = stopB;
					stopB = &stop;
					break;
				}
				pointA = pointB;
				stopA = stopB;
				stopB = &stop;
			}

			if (stopB == nullptr)
			{
				return stopA != nullptr ? stopA->GetColor() : SvgColor();
			}

			if (stopA == nullptr) return {};

			auto distanceFromPoint = equation.GetDistanceFromPoint(point);

			auto distance1 = b.GetDistance(point);

			auto distance2 = sqrt(std::abs(distanceFromPoint * distanceFromPoint - distance1 * distance1));

			auto value = distance2 / distance;

			auto tR = stopA->GetColor().R() * value;
			auto tG = stopA->GetColor().G() * value;
			auto tB = stopA->GetColor().B() * value;
			auto tA = (stopA->GetColor().A() * stopA->GetOpacity()) * value;

			tR += stopB->GetColor().R() * (1.0 - value);
			tG += stopB->GetColor().G() * (1.0 - value);
			tB += stopB->GetColor().B() * (1.0 - value);
			tA += (stopB->GetColor().A() * stopB->GetOpacity()) * (1.0 - value);

			return SvgColor(tR, tG, tB, tA);
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
