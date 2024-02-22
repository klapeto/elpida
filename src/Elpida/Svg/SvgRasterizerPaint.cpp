//
// Created by klapeto on 27/1/2024.
//

#include "Elpida/Svg/SvgRasterizerPaint.hpp"

#include <assert.h>
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

		auto& stops = _stopsGradient->GetStops();
		if (stops.empty()) return {};
		if (stops.size() == 1) return stops.front().GetColor();

		if (_gradient->GetType() == SvgGradientType::Linear)
		{
			switch (_gradient->GetSpreadType())
			{
			case SvgSpreadType::Pad:
				return CalculateLinearGradientPad(point, document);
			case SvgSpreadType::Repeat:
				return CalculateLinearGradientRepeat(point, document);
			case SvgSpreadType::Reflect:
				return CalculateLinearGradientReflect(point, document);
			}
		} else
		{
			switch (_gradient->GetSpreadType())
			{
			case SvgSpreadType::Pad:
				return CalculateRadialGradientPad(point, document);
			case SvgSpreadType::Repeat:
				return CalculateRadialGradientRepeat(point, document);
			case SvgSpreadType::Reflect:
				return CalculateRadialGradientReflect(point, document);
			}
		}

		return {};
	}

	static SvgColor CalculateColorFromStops(const SvgPoint& point,
		const SvgLinearEquation& gradientEquation,
		const SvgGradientStop& stopA,
		const SvgLinearEquation& normalA,
		const SvgGradientStop& stopB,
		const SvgLinearEquation& normalB)
	{
		const auto perpedicularEquation = gradientEquation.GetPerpendicularEquationFromPoint(point);

		const auto distanceFromB = normalB.GetP1().GetDistance(perpedicularEquation.GetP2());
		const auto stopDistance = normalB.GetP1().GetDistance(normalA.GetP1());
		const auto ratio = distanceFromB > stopDistance ? stopDistance / distanceFromB : distanceFromB / stopDistance;

		auto tR = stopA.GetColor().R() * ratio;
		auto tG = stopA.GetColor().G() * ratio;
		auto tB = stopA.GetColor().B() * ratio;
		auto tA = (stopA.GetColor().A() * stopA.GetOpacity()) * ratio;

		tR += stopB.GetColor().R() * (1.0 - ratio);
		tG += stopB.GetColor().G() * (1.0 - ratio);
		tB += stopB.GetColor().B() * (1.0 - ratio);
		tA += (stopB.GetColor().A() * stopB.GetOpacity()) * (1.0 - ratio);

		return SvgColor(tR, tG, tB, tA);
	}

	SvgRasterizerPaint::SvgRasterizerPaint(const SvgPaint& paint, const SvgElement& element, const SvgDocument& document)
		: _gradient(nullptr), _stopsGradient(nullptr)
	{
		if (paint.GetGradientId().empty())
		{
			AsColor(paint);
		}
		else
		{
			AsGradient(paint, element, document);
		}
	}

	SvgRasterizerPaint::~SvgRasterizerPaint()
	{
	}

	void SvgRasterizerPaint::AsGradient(const SvgPaint& paint, const SvgElement& element, const SvgDocument& document)
	{
		// TODO: make objectBoundBox coordinates work.
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
				const auto elementBounds = _gradient->GetUnits() == SvgGradientUnits::User ? SvgBounds(0.0,0.0,1.0,1.0) : element.GetBounds();
				auto& data = _gradient->GetData().linear;
				auto gradientPointA = SvgPoint(
					data.x1.CalculateActualValue(document, 0.0, elementBounds.GetWidth()),
					data.y1.CalculateActualValue(document, 0.0, elementBounds.GetHeight())
				);

				auto grandientPointB = SvgPoint(
					data.x2.CalculateActualValue(document, 0.0, elementBounds.GetWidth()),
					data.y2.CalculateActualValue(document, 0.0, elementBounds.GetHeight())
				);

				gradientPointA.Transform(_gradient->GetGradientTransform());
				grandientPointB.Transform(_gradient->GetGradientTransform());

				_gradientCache = LinearCache{
					SvgLinearEquation(gradientPointA, grandientPointB),
					std::vector<SvgLinearEquation>()
				};
				auto& linear = std::get<LinearCache>(_gradientCache);

				const auto& equation = linear.equation;
				auto& stopNormals = linear.stopNormals;
				stopNormals.reserve(stops.size());

				stopNormals.push_back(equation.GetANormal());
				linear.length = gradientPointA.GetDistance(grandientPointB);

				for (std::size_t i = 1; i < stops.size() - 1; ++i)
				{
					auto& stop = stops[i];

					// calculate the linear interpolation (where x,y exactly lies between the points)
					const auto x = std::lerp(gradientPointA.GetX(), grandientPointB.GetX(), stop.GetOffset());
					const auto y = std::lerp(gradientPointA.GetY(), grandientPointB.GetY(), stop.GetOffset());

					stopNormals.push_back(SvgLinearEquation(gradientPointA, SvgPoint(x, y)).GetBNormal());
				}

				stopNormals.push_back(equation.GetBNormal());
			}
			else
			{
				const auto elementBounds = _gradient->GetUnits() == SvgGradientUnits::User ? SvgBounds(0.0,0.0,1.0,1.0) : element.GetBounds();
				auto& data = _gradient->GetData().radial;

				auto radius = data.r.CalculateActualValue(document, 0.0, elementBounds.GetWidth());
				auto cX = data.cx.CalculateActualValue(document,0.0, elementBounds.GetWidth());
				auto cY = data.cy.CalculateActualValue(document,0.0, elementBounds.GetHeight());
				SvgEllipseEquation equation(
					cX,
					cY,
					radius,
					radius);

				//equation.Transform(_gradient->GetGradientTransform());

				_gradientCache = RadialCache{
					std::vector<SvgEllipseEquation>()
				};

				auto& radial = std::get<RadialCache>(_gradientCache);
				auto& stopEllipses = radial.stopEllipses;

				for (std::size_t i = 0; i < stops.size(); ++i)
				{
					auto& stop = stops[i];

					// calculate the linear interpolation (where x,y exactly lies between the points)
					const auto thisRadius = std::lerp(0, radius, stop.GetOffset());

					stopEllipses.emplace_back(cX, cY, thisRadius, thisRadius).Transform(_gradient->GetGradientTransform());
				}

				//equation.Transform()

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

			if (normalA->IsPointBehindLine(point, linear.equation.GetDirection()))
			{
				break;
			}

			stopA = &stops[i];

			if (normalB->IsPointBehindLine(point, linear.equation.GetDirection()))
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

		return CalculateColorFromStops(point, linear.equation, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgRasterizerPaint::CalculateLinearGradientRepeat(const SvgPoint& point, const SvgDocument& document) const
	{
		auto& stops = _stopsGradient->GetStops();

		auto& linear = std::get<LinearCache>(_gradientCache);
		auto stopNormals = linear.stopNormals;		// copy

		SvgTransform transform;

		const auto distanceA = stopNormals.front().GetDistanceFromPoint(point);
		const auto distanceB = stopNormals.back().GetDistanceFromPoint(point);
		const auto distance = linear.equation.GetP1().GetDistance(linear.equation.GetP2());

		if (distanceA > distance || distanceB > distance)
		{
			if (distanceA < distanceB)
			{
				const int ratio = 1 + distanceA / distance;
				const auto delta = linear.equation.GetP2() - linear.equation.GetP1();
				transform.SetTranslation(-(delta.GetX() * ratio), -(delta.GetY() * ratio));
			}
			else
			{
				const int ratio = 1 + distanceB / distance;
				const auto delta = linear.equation.GetP2() - linear.equation.GetP1();
				transform.SetTranslation(delta.GetX() * ratio, delta.GetY() * ratio);
			}
		}

		for (auto& normal: stopNormals)
		{
			normal.Transform(transform);
		}

		const SvgGradientStop* stopA = nullptr;
		const SvgGradientStop* stopB = nullptr;
		const SvgLinearEquation* normalA = nullptr;
		const SvgLinearEquation* normalB = nullptr;

		for (std::size_t i = 0; i < stops.size() - 1; i++)
		{
			normalA = &stopNormals[i];
			normalB = &stopNormals[i + 1];

			stopA = &stops[i];

			if (normalB->IsPointBehindLine(point, linear.equation.GetDirection()))
			{
				stopB = &stops[i + 1];
				break;
			}
		}
		assert(normalA != nullptr);
		assert(normalB != nullptr);
		return CalculateColorFromStops(point, linear.equation, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgRasterizerPaint::CalculateLinearGradientReflect(const SvgPoint& point,
	                                                            const SvgDocument& document) const
	{
		auto& stops = _stopsGradient->GetStops();

		auto& linear = std::get<LinearCache>(_gradientCache);
		auto equation = linear.equation;
		auto stopNormals = linear.stopNormals;		// copy

		SvgTransform transform;

		const auto distanceA = stopNormals.front().GetDistanceFromPoint(point);
		const auto distanceB = stopNormals.back().GetDistanceFromPoint(point);
		const auto distance = linear.equation.GetP1().GetDistance(linear.equation.GetP2());

		bool inverse = false;
		if (distanceA > distance || distanceB > distance)
		{
			if (distanceA < distanceB)
			{
				const int ratio = 1 + distanceA / distance;
				inverse = ratio % 2 != 0;
				const auto delta = linear.equation.GetP2() - linear.equation.GetP1();
				transform.SetTranslation(-(delta.GetX() * ratio), -(delta.GetY() * ratio));
			}
			else
			{
				const int ratio = 1 + distanceB / distance;
				const auto delta = linear.equation.GetP2() - linear.equation.GetP1();
				inverse = ratio % 2 != 0;
				transform.SetTranslation(delta.GetX() * ratio, delta.GetY() * ratio);
			}
		}

		auto direction = equation.GetDirection();
		if (inverse)
		{
			SvgTransform flip;
			auto dx = -std::min(equation.GetP1().GetX(), equation.GetP2().GetX()) - std::abs(equation.GetP1().GetX() - equation.GetP2().GetX()) / 2.0;
			auto dy = -std::min(equation.GetP1().GetY(), equation.GetP2().GetY()) - std::abs(equation.GetP1().GetY() - equation.GetP2().GetY()) / 2.0;

			// we need to move the gradient bound box to the center of the axis system
			// before rotate and move it back
			flip.Translate(dx, dy)
				.RotateDegrees(180)
				.Translate(-dx, -dy);

			//equation.Transform(flip);
			direction = -direction;
			transform.Multiply(flip);
		}

		for (auto& normal: stopNormals)
		{
			normal.Transform(transform);
		}

		const SvgGradientStop* stopA = nullptr;
		const SvgGradientStop* stopB = nullptr;
		const SvgLinearEquation* normalA = nullptr;
		const SvgLinearEquation* normalB = nullptr;

		for (std::size_t i = 0; i < stops.size() - 1; i++)
		{
			normalA = &stopNormals[i];
			normalB = &stopNormals[i + 1];

			stopA = &stops[i];

			if (normalB->IsPointBehindLine(point, direction))
			{
				stopB = &stops[i + 1];
				break;
			}
		}

		assert(stopA != nullptr);
		assert(stopB != nullptr);
		return CalculateColorFromStops(point, equation, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgRasterizerPaint::CalculateRadialGradientPad(const SvgPoint& point, const SvgDocument& document) const
	{
		auto& stops = _stopsGradient->GetStops();

		auto& radial = std::get<RadialCache>(_gradientCache);
		auto& stopEquations = radial.stopEllipses;

		const SvgGradientStop* stopA = nullptr;
		const SvgGradientStop* stopB = nullptr;

		const SvgEllipseEquation* equationA = nullptr;
		const SvgEllipseEquation* equationB= nullptr;

		for (std::size_t i = 0; i < stops.size() - 1; i++)
		{
			stopA = &stops[i];
			equationA = &stopEquations[i];
			equationB = &stopEquations[i + 1];

			if (stopEquations[i + 1].IsPointInside(point))
			{
				stopB = &stops[i+1];
				break;
			}
		}

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

		auto distanceFromA = equationA->GetFocusA().GetDistance(point) + equationA->GetFocusB().GetDistance(point);
		auto stopDistance = equationB->GetConstantDistance() - equationA->GetConstantDistance();

		const auto ratio = (equationB->GetConstantDistance() - distanceFromA) / stopDistance;

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

	SvgColor SvgRasterizerPaint::CalculateRadialGradientRepeat(const SvgPoint& point, const SvgDocument& document) const
	{
	}

	SvgColor SvgRasterizerPaint::CalculateRadialGradientReflect(const SvgPoint& point,
		const SvgDocument& document) const
	{
	}
} // Elpida
