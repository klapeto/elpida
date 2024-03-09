//
// Created by klapeto on 27/1/2024.
//

#include "Elpida/Svg/SvgCalculatedPaint.hpp"

#include <cassert>
#include <ranges>

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
		if (_stops.empty())
		{
			return _color;
		}

		if (_stops.size() == 1) return _stops.front().GetColor();

		// TODO: Create a delegate: Measure impact
		if (_gradientType == SvgGradientType::Linear)
		{
			switch (_spreadType)
			{
			case SvgSpreadType::Pad:
				return CalculateLinearGradientPad(point);
			case SvgSpreadType::Repeat:
				return CalculateLinearGradientRepeat(point);
			case SvgSpreadType::Reflect:
				return CalculateLinearGradientReflect(point);
			}
		}
		else
		{
			switch (_spreadType)
			{
			case SvgSpreadType::Pad:
				return CalculateRadialGradientPad(point);
			case SvgSpreadType::Repeat:
				return CalculateRadialGradientRepeat(point);
			case SvgSpreadType::Reflect:
				return CalculateRadialGradientReflect(point);
			}
		}

		return {};
	}

	SvgColor SvgCalculatedPaint::CalculateColorForLinear(const SvgPoint& point,
			const SvgLinearEquation& gradientEquation,
			const SvgCalculatedGradientStop& stopA,
			const SvgLinearEquation& normalA,
			const SvgCalculatedGradientStop& stopB,
			const SvgLinearEquation& normalB)
	{
		const auto perpendicularEquation = gradientEquation.GetPerpendicularEquationFromPoint(point);

		const auto distanceFromB = normalB.GetP1().GetDistance(perpendicularEquation.GetP2());
		const auto stopDistance = normalB.GetP1().GetDistance(normalA.GetP1());
		const auto ratio = distanceFromB > stopDistance ? stopDistance / distanceFromB : distanceFromB / stopDistance;

		return InterpolateColor(stopA, stopB, ratio);
	}

	SvgCalculatedPaint::SvgCalculatedPaint(const SvgPaint& paint,
			const SvgBounds& elementBounds,
			const SvgDocument& document,
			const SvgCalculationContext& calculationContext)
			:_gradientType(SvgGradientType::Linear), _spreadType(SvgSpreadType::Pad)
	{
		if (paint.GetGradientId().empty())
		{
			AsColor(paint);
		}
		else
		{
			AsGradient(paint, elementBounds, document, calculationContext);
		}
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

		CalculateStops(*stopsGradient, calculationContext);
		_gradientType = gradient->GetType();
		_spreadType = gradient->GetSpreadType();

		if (_gradientType == SvgGradientType::Linear)
		{
			const auto bounds = gradient->GetUnits() == SvgGradientUnits::User
								? SvgBounds(0.0, 0.0, 1.0, 1.0)
								: elementBounds;
			auto& data = gradient->GetData().linear;
			auto gradientPointA = SvgPoint(
					data.x1.CalculateValue(calculationContext, bounds.GetWidth()),
					data.y1.CalculateValue(calculationContext, bounds.GetHeight())
			);

			auto gradientPointB = SvgPoint(
					data.x2.CalculateValue(calculationContext, bounds.GetWidth()),
					data.y2.CalculateValue(calculationContext, bounds.GetHeight())
			);

			gradientPointA.Transform(gradient->GetGradientTransform());
			gradientPointB.Transform(gradient->GetGradientTransform());

			_gradientCache = LinearCache{
					SvgLinearEquation(gradientPointA, gradientPointB),
					std::vector<SvgLinearEquation>(),
					gradientPointA.GetDistance(gradientPointB)
			};
			auto& linear = std::get<LinearCache>(_gradientCache);

			const auto& equation = linear.equation;
			auto& stopNormals = linear.stopNormals;
			stopNormals.reserve(_stops.size());

			stopNormals.push_back(equation.GetANormal());

			for (std::size_t i = 1; i < _stops.size() - 1; ++i)
			{
				auto& stop = _stops[i];

				// calculate the linear interpolation (where x,y exactly lies between the points)
				const auto x = std::lerp(gradientPointA.GetX(), gradientPointB.GetX(), stop.GetOffset());
				const auto y = std::lerp(gradientPointA.GetY(), gradientPointB.GetY(), stop.GetOffset());

				stopNormals.push_back(SvgLinearEquation(gradientPointA, SvgPoint(x, y)).GetBNormal());
			}

			stopNormals.push_back(equation.GetBNormal());
		}
		else
		{
			const auto bounds = gradient->GetUnits() == SvgGradientUnits::User
								? SvgBounds(0.0, 0.0, 1.0, 1.0)
								: elementBounds;
			auto& data = gradient->GetData().radial;

			const auto actualRadius = data.r.CalculateValue(calculationContext, bounds.GetWidth());
			auto actualCX = data.cx.CalculateValue(calculationContext, bounds.GetWidth());
			auto actualCY = data.cy.CalculateValue(calculationContext, bounds.GetHeight());

			_gradientCache = RadialCache{
					std::vector<SvgEllipseEquation>()
			};

			auto& radial = std::get<RadialCache>(_gradientCache);
			auto& stopEllipses = radial.stopEllipses;

			stopEllipses.reserve(_stops.size());

			for (const auto& stop : _stops)
			{
				// calculate the linear interpolation of the radius
				const auto thisRadius = std::lerp(0, actualRadius, stop.GetOffset());

				stopEllipses.emplace_back(actualCX, actualCY, thisRadius, thisRadius)
						.Transform(gradient->GetGradientTransform());
			}
		}
	}

	void SvgCalculatedPaint::AsColor(const SvgPaint& paint)
	{
		_color = paint.GetColor();
	}

	SvgColor SvgCalculatedPaint::CalculateLinearGradientPad(const SvgPoint& point) const
	{
		auto& linear = std::get<LinearCache>(_gradientCache);
		auto& stopNormals = linear.stopNormals;

		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;
		const SvgLinearEquation* normalA = nullptr;
		const SvgLinearEquation* normalB = nullptr;

		for (std::size_t i = 0; i < _stops.size(); i++)
		{
			normalA = &stopNormals[i];
			normalB = &stopNormals[i + 1];

			if (normalA->IsPointBehindLine(point, linear.equation.GetDirection()))
			{
				break;
			}

			stopA = &_stops[i];

			if (normalB->IsPointBehindLine(point, linear.equation.GetDirection()))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		// the point is not before any stop
		if (stopA == nullptr)
		{
			auto& firstStop = _stops.front();
			return firstStop.GetColor().WithMultipliedAplha(firstStop.GetOpacity());
		}

		// the point is beyond all stops
		if (stopB == nullptr)
		{
			auto& lastStop = _stops.back();
			return lastStop.GetColor().WithMultipliedAplha(lastStop.GetOpacity());
		}

		return CalculateColorForLinear(point, linear.equation, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgCalculatedPaint::CalculateLinearGradientRepeat(const SvgPoint& point) const
	{
		auto& linear = std::get<LinearCache>(_gradientCache);
		auto stopNormals = linear.stopNormals; // copy

		SvgTransform transform;

		const auto distanceFromA = stopNormals.front().GetDistanceFromPoint(point);
		const auto distanceFromB = stopNormals.back().GetDistanceFromPoint(point);
		const auto stopDistance = linear.length;

		if (distanceFromA > stopDistance || distanceFromB > stopDistance)
		{
			if (distanceFromA < distanceFromB)
			{
				const int ratio = 1 + distanceFromA / stopDistance;
				const auto delta = linear.equation.GetP2() - linear.equation.GetP1();
				transform.SetTranslation(-(delta.GetX() * ratio), -(delta.GetY() * ratio));
			}
			else
			{
				const int ratio = 1 + distanceFromB / stopDistance;
				const auto delta = linear.equation.GetP2() - linear.equation.GetP1();
				transform.SetTranslation(delta.GetX() * ratio, delta.GetY() * ratio);
			}
		}

		for (auto& normal : stopNormals)
		{
			normal.Transform(transform);
		}

		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;
		const SvgLinearEquation* normalA = nullptr;
		const SvgLinearEquation* normalB = nullptr;

		for (std::size_t i = 0; i < _stops.size() - 1; i++)
		{
			normalA = &stopNormals[i];
			normalB = &stopNormals[i + 1];

			stopA = &_stops[i];

			if (normalB->IsPointBehindLine(point, linear.equation.GetDirection()))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		assert(normalA != nullptr);
		assert(normalB != nullptr);

		return CalculateColorForLinear(point, linear.equation, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgCalculatedPaint::CalculateLinearGradientReflect(const SvgPoint& point) const
	{
		auto& linear = std::get<LinearCache>(_gradientCache);
		auto equation = linear.equation;
		auto stopNormals = linear.stopNormals; // copy

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
			auto dx = -std::min(equation.GetP1().GetX(), equation.GetP2().GetX()) - std::abs(
					equation.GetP1().GetX() - equation.GetP2().GetX()) / 2.0;
			auto dy = -std::min(equation.GetP1().GetY(), equation.GetP2().GetY()) - std::abs(
					equation.GetP1().GetY() - equation.GetP2().GetY()) / 2.0;

			// we need to move the gradient bound box to the center of the axis system
			// before rotate and move it back
			flip.Translate(dx, dy)
					.RotateDegrees(180)
					.Translate(-dx, -dy);

			//equation.Transform(flip);
			direction = -direction;
			transform.Multiply(flip);
		}

		for (auto& normal : stopNormals)
		{
			normal.Transform(transform);
		}

		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;
		const SvgLinearEquation* normalA = nullptr;
		const SvgLinearEquation* normalB = nullptr;

		for (std::size_t i = 0; i < _stops.size() - 1; i++)
		{
			normalA = &stopNormals[i];
			normalB = &stopNormals[i + 1];

			stopA = &_stops[i];

			if (normalB->IsPointBehindLine(point, direction))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		assert(stopA != nullptr);
		assert(stopB != nullptr);
		return CalculateColorForLinear(point, equation, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgCalculatedPaint::InterpolateColor(const SvgCalculatedGradientStop& stopA,
			const SvgCalculatedGradientStop& stopB,
			double ratio)
	{
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

	SvgColor SvgCalculatedPaint::CalculateRadialGradientPad(const SvgPoint& point) const
	{
		auto& radial = std::get<RadialCache>(_gradientCache);
		auto& stopEquations = radial.stopEllipses;

		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;

		const SvgEllipseEquation* equationA = nullptr;
		const SvgEllipseEquation* equationB = nullptr;

		for (std::size_t i = 0; i < _stops.size() - 1; i++)
		{
			stopA = &_stops[i];
			equationA = &stopEquations[i];
			equationB = &stopEquations[i + 1];

			if (stopEquations[i + 1].IsPointInside(point))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		if (stopA == nullptr)
		{
			auto& firstStop = _stops.front();
			return firstStop.GetColor().WithMultipliedAplha(firstStop.GetOpacity());
		}

		// the point is beyond all stops
		if (stopB == nullptr)
		{
			auto& lastStop = _stops.back();
			return lastStop.GetColor().WithMultipliedAplha(lastStop.GetOpacity());
		}

		auto closestPointToA = equationA->CalculateClosestPoint(point);
		auto closestPointToB = equationB->CalculateClosestPoint(point);

		double distanceFromB = closestPointToB.GetDistance(point);

		auto ratio = closestPointToA == point && closestPointToA != equationA->GetCenter()
					 ? 1.0
					 : distanceFromB / closestPointToA.GetDistance(closestPointToB);

		return InterpolateColor(*stopA, *stopB, ratio);
	}

	SvgColor SvgCalculatedPaint::CalculateRadialGradientRepeat(const SvgPoint& point) const
	{
		auto& radial = std::get<RadialCache>(_gradientCache);
		auto stopEquations = radial.stopEllipses; // Copy

		const auto& lastStopEquation = stopEquations.back();
		const auto closePointToGradient = lastStopEquation.CalculateClosestPoint(point);
		const auto pointDistanceFromCenter = lastStopEquation.GetCenter().GetDistance(point);
		const auto ellipsePointDistanceFromCenter = lastStopEquation.GetCenter().GetDistance(closePointToGradient);

		if (pointDistanceFromCenter > ellipsePointDistanceFromCenter)
		{
			const auto distanceFromB = closePointToGradient.GetDistance(point);
			const int ratio = (1 + (distanceFromB / ellipsePointDistanceFromCenter));
			const auto rXToAdd = lastStopEquation.GetA() * ratio;
			const auto ryToAdd = lastStopEquation.GetB() * ratio;

			for (auto& stopEquation : stopEquations)
			{
				stopEquation.Expand(rXToAdd, ryToAdd, lastStopEquation.GetAngle());
			}
		}

		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;

		const SvgEllipseEquation* equationA = nullptr;
		const SvgEllipseEquation* equationB = nullptr;

		for (std::size_t i = 0; i < _stops.size() - 1; i++)
		{
			stopA = &_stops[i];
			equationA = &stopEquations[i];
			equationB = &stopEquations[i + 1];

			if (stopEquations[i + 1].IsPointInside(point))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		if (stopA == nullptr)
		{
			auto& firstStop = _stops.front();
			return firstStop.GetColor().WithMultipliedAplha(firstStop.GetOpacity());
		}

		// the point is beyond all stops
		if (stopB == nullptr)
		{
			auto& lastStop = _stops.back();
			return lastStop.GetColor().WithMultipliedAplha(lastStop.GetOpacity());
		}

		auto closestPointToA = equationA->CalculateClosestPoint(point);
		auto closestPointToB = equationB->CalculateClosestPoint(point);

		double distanceFromB = closestPointToB.GetDistance(point);

		auto ratio = closestPointToA == point && closestPointToA != equationA->GetCenter()
					 ? 1.0
					 : distanceFromB / closestPointToA.GetDistance(closestPointToB);

		return InterpolateColor(*stopA, *stopB, ratio);
	}

	SvgColor SvgCalculatedPaint::CalculateRadialGradientReflect(const SvgPoint& point) const
	{
		auto& radial = std::get<RadialCache>(_gradientCache);

		auto& originalEquations = radial.stopEllipses;

		std::vector<SvgEllipseEquation> stopEquations;
		std::vector<std::reference_wrapper<const SvgCalculatedGradientStop>> stops;

		stops.reserve(originalEquations.size());
		stopEquations.reserve(originalEquations.size());

		const auto& lastStopEquation = originalEquations.back();
		const auto closePointToGradient = lastStopEquation.CalculateClosestPoint(point);
		const auto pointDistanceFromCenter = lastStopEquation.GetCenter().GetDistance(point);
		const auto ellipsePointDistanceFromCenter = lastStopEquation.GetCenter().GetDistance(closePointToGradient);

		if (pointDistanceFromCenter > ellipsePointDistanceFromCenter)
		{
			const auto distanceFromB = closePointToGradient.GetDistance(point);
			const int ratio = (1 + (distanceFromB / ellipsePointDistanceFromCenter));
			const auto rXToAdd = lastStopEquation.GetA() * ratio;
			const auto ryToAdd = lastStopEquation.GetB() * ratio;

			auto inverted = ratio % 2 != 0;
			if (inverted)
			{
				auto size = originalEquations.size();

				for (std::size_t i = 0; i < size; ++i)
				{
					stopEquations.push_back(originalEquations[size - i - 1]);
					stopEquations.back().Expand(rXToAdd, ryToAdd, lastStopEquation.GetAngle());
					stops.emplace_back(_stops[size - i - 1]);
				}

				SvgBounds topBounds(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
						std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest());
				SvgBounds rightBounds(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
						std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest());

				for (auto& equation : stopEquations)
				{
					rightBounds.Merge(SvgBounds(equation.GetRightPoint()));
					topBounds.Merge(SvgBounds(equation.GetTopPoint()));
				}

				auto topTranslateX = topBounds.GetMinX() + (topBounds.GetWidth() / 2.0);
				auto topTranslateY = topBounds.GetMinY() + (topBounds.GetHeight() / 2.0);
				SvgTransform topTransform;
				topTransform.Translate(-topTranslateX, -topTranslateY)
						.RotateDegrees(180)
						.Translate(topTranslateX, topTranslateY);

				auto rightTranslateX = rightBounds.GetMinX() + (rightBounds.GetWidth() / 2.0);
				auto rightTranslateY = rightBounds.GetMinY() + (rightBounds.GetHeight() / 2.0);
				SvgTransform rightTransform;
				rightTransform.Translate(-rightTranslateX, -rightTranslateY)
						.RotateDegrees(180)
						.Translate(rightTranslateX, rightTranslateY);

				for (auto& stopEquation : stopEquations)
				{
					auto& equation = stopEquation;
					auto right = equation.GetRightPoint();
					auto top = equation.GetTopPoint();
					right.Transform(rightTransform);
					top.Transform(topTransform);
					stopEquation = SvgEllipseEquation(equation.GetCenter(), top, right);
				}
			}
			else
			{
				for (std::size_t i = 0; i < originalEquations.size(); ++i)
				{
					stopEquations.push_back(originalEquations[i]);
					stopEquations.back().Expand(rXToAdd, ryToAdd, lastStopEquation.GetAngle());
					stops.emplace_back(_stops[i]);
				}
			}
		}
		else
		{
			for (std::size_t i = 0; i < originalEquations.size(); ++i)
			{
				stopEquations.push_back(originalEquations[i]);
				stops.emplace_back(_stops[i]);
			}
		}

		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;

		const SvgEllipseEquation* equationA = nullptr;
		const SvgEllipseEquation* equationB = nullptr;

		for (std::size_t i = 0; i < stops.size() - 1; i++)
		{
			stopA = &stops[i].get();
			equationA = &stopEquations[i];
			equationB = &stopEquations[i + 1];

			if (stopEquations[i + 1].IsPointInside(point))
			{
				stopB = &stops[i + 1].get();
				break;
			}
		}

		if (stopA == nullptr)
		{
			auto& firstStop = stops.front();
			return firstStop.get().GetColor().WithMultipliedAplha(firstStop.get().GetOpacity());
		}

		// the point is beyond all stops
		if (stopB == nullptr)
		{
			auto& lastStop = stops.back();
			return lastStop.get().GetColor().WithMultipliedAplha(lastStop.get().GetOpacity());
		}

		auto closestPointToA = equationA->CalculateClosestPoint(point);
		auto closestPointToB = equationB->CalculateClosestPoint(point);

		double distanceFromB = closestPointToB.GetDistance(point);

		auto ratio = closestPointToA == point && closestPointToA != equationA->GetCenter()
					 ? 1.0
					 : distanceFromB / closestPointToA.GetDistance(closestPointToB);

		return InterpolateColor(*stopA, *stopB, ratio);
	}

	void SvgCalculatedPaint::CalculateStops(const SvgGradient& gradientStops,
			const SvgCalculationContext& calculationContext)
	{
		for (auto& stop : gradientStops.GetStops())
		{
			_stops.emplace_back(stop.GetColor(),
					stop.GetOffset().CalculateValue(calculationContext, 1.0),
					stop.GetOpacity().CalculateValue(calculationContext, 1.0));
		}

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
			:_gradientType(SvgGradientType::Linear), _spreadType(SvgSpreadType::Pad)
	{

	}
} // Elpida
