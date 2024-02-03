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
			switch (_gradient->GetSpreadType())
			{
			case SvgSpreadType::Pad:
				return CalculateLinearGradientPad(point, document);
			case SvgSpreadType::Repeat:
				return CalculateLinearGradientRepeat(point, document);
			case SvgSpreadType::Reflect:
				return CalculateLinearGradientPad(point, document);
			}
		}

		return {};
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

	SvgRasterizerPaint::~SvgRasterizerPaint()
	{

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

			auto& stops = _stopsGradient->GetStops();
			if (_gradient->GetType() == SvgGradientType::Linear)
			{
				auto& data = _gradient->GetData().linear;
				auto a = SvgPoint(
					data.x1.CalculateActualValue(document, 0.0, 1.0),
					data.y1.CalculateActualValue(document, 0.0, 1.0)
				);

				auto b = SvgPoint(
					data.x2.CalculateActualValue(document, 0.0, 1.0),
					data.y2.CalculateActualValue(document, 0.0, 1.0)
				);

				a.ApplyTransform(_gradient->GetGradientTransform());
				b.ApplyTransform(_gradient->GetGradientTransform());

				_gradientCache = LinearCache{
					SvgLinearEquation(a,b),
					std::vector<SvgLinearEquation>()
				};
				auto& linear = std::get<LinearCache>(_gradientCache);

				const auto& equation = linear.equation;
				auto& stopNormals = linear.stopNormals;
				stopNormals.reserve(stops.size());

				stopNormals.push_back(equation.GetANormal());

				auto x1 = a.GetX();

				for (std::size_t i = 1; i < stops.size() - 1; ++i)
				{
					auto& stop = stops[i];
					const auto x = x1 + (x1 * stop.GetOffset());
					const auto y = equation.CalculateY(x);

					stopNormals.push_back(SvgLinearEquation(a, SvgPoint(x,y)).GetBNormal());
				}

				stopNormals.push_back(equation.GetBNormal());
			}
			else
			{

			}
		}
	}

	void SvgRasterizerPaint::AsColor(const SvgPaint& paint)
	{
		_color = paint.GetColor();
	}

	SvgColor SvgRasterizerPaint::CalculateLinearGradientPad(const SvgPoint& point, const SvgDocument& document) const
	{
		auto& stops = _stopsGradient->GetStops();
		if (stops.empty()) return {};
		if (stops.size() == 1) return stops.front().GetColor();

		auto& linear = std::get<LinearCache>(_gradientCache);
		auto& stopNormals = linear.stopNormals;

		const SvgGradientStop* stopA = nullptr;
		const SvgGradientStop* stopB = nullptr;
		const SvgLinearEquation* normalA = nullptr;
		const SvgLinearEquation* normalB = nullptr;

		for (std::size_t i = 0; i < stops.size(); i++)
		{
			normalA = &stopNormals[i];
			normalB = &stopNormals[i + 1];

			if (normalA->IsPointLeftOf(point))
			{
				break;
			}

			stopA = &stops[i];

			if (normalB->IsPointLeftOf(point))
			{
				stopB = &stops[i + 1];
				break;
			}
		}

		// the point is not before any stop
		if (stopA == nullptr)
		{
			auto& firstStop = stops.front();
			return firstStop.GetColor().WithMultipliedAplha(firstStop.GetOpacity());
		}

		// the point is beyond all stops
		if (stopB == nullptr)
		{
			auto& lastStop = stops.back();
			return lastStop.GetColor().WithMultipliedAplha(lastStop.GetOpacity());
		}

		const auto perpedicularEquation = linear.equation.GetPerpendicularEquationFromPoint(point);

		const auto distanceFromB = normalB->GetP1().GetDistance(perpedicularEquation.GetP2());
		const auto stopDistance = normalB->GetP1().GetDistance(normalA->GetP1());
		const auto ratio = distanceFromB / stopDistance;

		auto tR = stopA->GetColor().R() * ratio;
		auto tG = stopA->GetColor().G() * ratio;
		auto tB = stopA->GetColor().B() * ratio;
		auto tA = (stopA->GetColor().A() * stopA->GetOpacity()) * ratio;

		tR += stopB->GetColor().R() * (1.0 - ratio);
		tG += stopB->GetColor().G() * (1.0 - ratio);
		tB += stopB->GetColor().B() * (1.0 - ratio);
		tA += (stopB->GetColor().A() * stopB->GetOpacity()) * (1.0 - ratio);

		return SvgColor(tR, tG, tB, tA);
	}

	SvgColor SvgRasterizerPaint::CalculateLinearGradientRepeat(const SvgPoint& point, const SvgDocument& document) const
	{
		return {};
	}

	SvgColor SvgRasterizerPaint::CalculateLinearGradientReflect(const SvgPoint& point,
	                                                            const SvgDocument& document) const
	{
		return {};
	}
} // Elpida
