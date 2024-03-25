//
// Created by klapeto on 10/3/2024.
//

#include "Elpida/Svg/SvgCalculatedLinearGradient.hpp"
#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgPoint.hpp"

#include <cmath>
#include <cassert>

namespace Elpida
{
	SvgCalculatedLinearGradient::SvgCalculatedLinearGradient(const SvgGradient& gradient,
			const SvgBounds& elementBounds,
			std::vector<SvgCalculatedGradientStop>&& stops,
			const SvgCalculationContext& calculationContext)
			:SvgCalculatedGradient(std::move(stops), gradient.GetSpreadType())
	{
		const auto bounds = gradient.GetUnits() == SvgGradientUnits::User
							? SvgBounds(0.0, 0.0, 1.0, 1.0)
							: elementBounds;
		auto& data = gradient.GetData().linear;
		auto gradientPointA = SvgPoint(
				data.x1.CalculateValue(calculationContext, bounds.GetWidth()),
				data.y1.CalculateValue(calculationContext, bounds.GetHeight())
		);

		auto gradientPointB = SvgPoint(
				data.x2.CalculateValue(calculationContext, bounds.GetWidth()),
				data.y2.CalculateValue(calculationContext, bounds.GetHeight())
		);

		gradientPointA.Transform(gradient.GetGradientTransform());
		gradientPointB.Transform(gradient.GetGradientTransform());

		_equation = SvgLinearEquation(gradientPointA, gradientPointB);
		_length = gradientPointA.GetDistance(gradientPointB);

		_stopNormals.reserve(_stops.size());

		_stopNormals.push_back(_equation.GetANormal());

		for (std::size_t i = 1; i < _stops.size() - 1; ++i)
		{
			auto& stop = _stops[i];

			// calculate the linear interpolation (where x,y exactly lies between the points)
			const auto x = std::lerp(gradientPointA.GetX(), gradientPointB.GetX(), stop.GetOffset());
			const auto y = std::lerp(gradientPointA.GetY(), gradientPointB.GetY(), stop.GetOffset());

			_stopNormals.push_back(SvgLinearEquation(gradientPointA, SvgPoint(x, y)).GetBNormal());
		}

		_stopNormals.push_back(_equation.GetBNormal());
	}

	void SvgCalculatedLinearGradient::Transform(const SvgTransform& transform)
	{
		_equation.Transform(transform);
		for (auto& normal : _stopNormals)
		{
			normal.Transform(transform);
		}
		_length = _equation.GetP1().GetDistance(_equation.GetP2());
	}

	SvgColor SvgCalculatedLinearGradient::CalculateColor(const SvgPoint& point) const
	{
		// TODO: Eliminate branch, measure impact
		switch (_spreadType)
		{
		case SvgSpreadType::Pad:
			return CalculatePad(point);
		case SvgSpreadType::Reflect:
			return CalculateReflect(point);
		case SvgSpreadType::Repeat:
			return CalculateRepeat(point);
		}
	}

	SvgColor SvgCalculatedLinearGradient::CalculatePad(const SvgPoint& point) const
	{
		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;
		const SvgLinearEquation* normalA = nullptr;
		const SvgLinearEquation* normalB = nullptr;

		for (std::size_t i = 0; i < _stops.size() - 1; i++)
		{
			normalA = &_stopNormals[i];
			normalB = &_stopNormals[i + 1];

			if (normalA->IsPointBehindLine(point, _equation.GetDirection()))
			{
				break;
			}

			stopA = &_stops[i];

			if (normalB->IsPointBehindLine(point, _equation.GetDirection()))
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

		return CalculateColor(point, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgCalculatedLinearGradient::CalculateColor(const SvgPoint& point,
			const SvgCalculatedGradientStop& stopA,
			const SvgLinearEquation& normalA,
			const SvgCalculatedGradientStop& stopB,
			const SvgLinearEquation& normalB) const
	{
		const auto perpendicularEquation = _equation.GetPerpendicularEquationFromPoint(point);

		const auto distanceFromB = normalB.GetP1().GetDistance(perpendicularEquation.GetP2());
		const auto stopDistance = normalB.GetP1().GetDistance(normalA.GetP1());
		const auto ratio = distanceFromB > stopDistance ? stopDistance / distanceFromB : distanceFromB / stopDistance;

		return InterpolateColor(stopA, stopB, ratio);
	}

	SvgColor SvgCalculatedLinearGradient::CalculateRepeat(const SvgPoint& point) const
	{
		auto stopNormals = _stopNormals; // copy

		SvgTransform transform;

		const auto distanceFromA = stopNormals.front().GetDistanceFromPoint(point);
		const auto distanceFromB = stopNormals.back().GetDistanceFromPoint(point);
		const auto stopDistance = _length;

		if (distanceFromA > stopDistance || distanceFromB > stopDistance)
		{
			if (distanceFromA < distanceFromB)
			{
				const int ratio = 1 + distanceFromA / stopDistance;
				const auto delta = _equation.GetP2() - _equation.GetP1();
				transform.SetTranslation(-(delta.GetX() * ratio), -(delta.GetY() * ratio));
			}
			else
			{
				const int ratio = 1 + distanceFromB / stopDistance;
				const auto delta = _equation.GetP2() - _equation.GetP1();
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

			if (normalB->IsPointBehindLine(point, _equation.GetDirection()))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		assert(normalA != nullptr);
		assert(normalB != nullptr);

		return CalculateColor(point, *stopA, *normalA, *stopB, *normalB);
	}

	SvgColor SvgCalculatedLinearGradient::CalculateReflect(const SvgPoint& point) const
	{
		auto stopNormals = _stopNormals; // copy

		SvgTransform transform;

		const auto distanceA = stopNormals.front().GetDistanceFromPoint(point);
		const auto distanceB = stopNormals.back().GetDistanceFromPoint(point);
		const auto distance = _equation.GetP1().GetDistance(_equation.GetP2());

		bool inverse = false;
		if (distanceA > distance || distanceB > distance)
		{
			if (distanceA < distanceB)
			{
				const int ratio = 1 + distanceA / distance;
				inverse = ratio % 2 != 0;
				const auto delta = _equation.GetP2() - _equation.GetP1();
				transform.SetTranslation(-(delta.GetX() * ratio), -(delta.GetY() * ratio));
			}
			else
			{
				const int ratio = 1 + distanceB / distance;
				const auto delta = _equation.GetP2() - _equation.GetP1();
				inverse = ratio % 2 != 0;
				transform.SetTranslation(delta.GetX() * ratio, delta.GetY() * ratio);
			}
		}

		auto direction = _equation.GetDirection();
		if (inverse)
		{
			SvgTransform flip;
			auto dx = -std::min(_equation.GetP1().GetX(), _equation.GetP2().GetX()) - std::abs(
					_equation.GetP1().GetX() - _equation.GetP2().GetX()) / 2.0;
			auto dy = -std::min(_equation.GetP1().GetY(), _equation.GetP2().GetY()) - std::abs(
					_equation.GetP1().GetY() - _equation.GetP2().GetY()) / 2.0;

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
		return CalculateColor(point, *stopA, *normalA, *stopB, *normalB);
	}
} // Elpida