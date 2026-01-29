/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 10/3/2024.
//

#include "Elpida/Svg/SvgCalculatedRadialGradient.hpp"

#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgUtilities.hpp"

#include <cassert>

namespace Elpida
{
	SvgCalculatedRadialGradient::SvgCalculatedRadialGradient(const SvgGradient &gradient,
	                                                         const SvgBounds &elementBounds,
	                                                         std::vector<SvgCalculatedGradientStop> &&stops,
	                                                         const SvgCalculationContext &calculationContext)
		: SvgCalculatedGradient(std::move(stops), gradient.GetSpreadType())
	{
		const auto bounds = gradient.GetUnits() == SvgGradientUnits::User
			                    ? SvgBounds(0.0, 0.0, 1.0, 1.0)
			                    : elementBounds;
		const auto &[cx, cy, r, fx, fy] = gradient.GetData().radial;

		const auto actualRadius = r.CalculateValue(calculationContext, bounds.GetWidth());
		const auto actualCX = cx.CalculateValue(calculationContext, bounds.GetWidth());
		const auto actualCY = cy.CalculateValue(calculationContext, bounds.GetHeight());

		_circles.reserve(_stops.size());
		_inverseCircles.reserve(_stops.size());

		for (const auto &stop: _stops)
		{
			// calculate the linear interpolation of the radius
			const auto thisRadius = SvgUtilities::Lerp(0, actualRadius, stop.GetOffset());
			const auto thisInverseRadius = SvgUtilities::Lerp(0, actualRadius, 1.0 - stop.GetOffset());

			_circles.emplace_back(SvgPoint(actualCX, actualCY), thisRadius);
			_inverseCircles.emplace_back(SvgPoint(actualCX, actualCY), thisInverseRadius);
		}
		 _transform.Inverse(gradient.GetGradientTransform());	// We will inverse transform the points to calculate the colors.
	}

	SvgColor SvgCalculatedRadialGradient::CalculateColor(const SvgPoint &point) const
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
		return {};
	}

	SvgColor SvgCalculatedRadialGradient::CalculatePad(const SvgPoint &point) const
	{
		const SvgCalculatedGradientStop *stopA = nullptr;
		const SvgCalculatedGradientStop *stopB = nullptr;

		const SvgCircleEquation *circleA = nullptr;
		const SvgCircleEquation *circleB = nullptr;

		auto actualPoint = point;
		actualPoint.Transform(_transform);	// Transform the point to non transformed cicle space

		auto size = _stops.size() - 1;
		for (std::size_t i = 0; i < size; i++)
		{
			stopA = &_stops[i];
			circleA = &_circles[i];
			circleB = &_circles[i + 1];

			if (_circles[i + 1].IsPointInside(actualPoint))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		if (stopA == nullptr)
		{
			auto &firstStop = _stops.front();
			return firstStop.GetColor().WithMultipliedAplha(firstStop.GetOpacity());
		}

		// the point is beyond all stops
		if (stopB == nullptr)
		{
			auto &lastStop = _stops.back();
			return lastStop.GetColor().WithMultipliedAplha(lastStop.GetOpacity());
		}

		return CalculateColorFromStops(actualPoint, *stopA, *stopB, *circleA, *circleB);
	}

	SvgColor SvgCalculatedRadialGradient::CalculateRepeat(const SvgPoint &point) const
	{
		auto actualPoint = point;
		actualPoint.Transform(_transform);	// Transform the point to non transformed cicle space

		auto& lastCircle = _circles.back();
		const auto distanceFromPoint = lastCircle.GetCenter().GetDistance(actualPoint);

		auto circles = _circles;	//copy
		if (distanceFromPoint > lastCircle.GetRadius())
		{
			const int ratio = distanceFromPoint / lastCircle.GetRadius();
			const auto rXToAdd = lastCircle.GetRadius() * SvgFloat(ratio);
			for (auto& circle : circles)
			{
				circle.ScaleBy(rXToAdd);
			}
		}

		const SvgCalculatedGradientStop *stopA = nullptr;
		const SvgCalculatedGradientStop *stopB = nullptr;

		const SvgCircleEquation *circleA = nullptr;
		const SvgCircleEquation *circleB = nullptr;

		auto size = _stops.size() - 1;
		for (std::size_t i = 0; i < size; i++)
		{
			stopA = &_stops[i];
			circleA = &circles[i];
			circleB = &circles[i + 1];

			if (circles[i + 1].IsPointInside(actualPoint))
			{
				stopB = &_stops[i + 1];
				break;
			}
		}

		assert(stopA != nullptr);
		assert(stopB != nullptr);

		return CalculateColorFromStops(actualPoint, *stopA, *stopB, *circleA, *circleB);
	}

	SvgColor SvgCalculatedRadialGradient::CalculateColorFromStops(
		const SvgPoint &actualPoint,
		const SvgCalculatedGradientStop &stopA,
		const SvgCalculatedGradientStop &stopB,
		const SvgCircleEquation &circleA,
		const SvgCircleEquation &circleB)
	{
		const auto distanceFromA = circleA.GetCenter().GetDistance(actualPoint);

		const auto stopDistance = circleB.GetRadius() - circleA.GetRadius();
		const auto distanceDifference = circleB.GetRadius() - distanceFromA;

		const auto ratio = distanceDifference / stopDistance;

		return InterpolateColor(stopA, stopB, ratio);
	}

	SvgColor SvgCalculatedRadialGradient::CalculateReflect(const SvgPoint &point) const
	{
		auto actualPoint = point;
		actualPoint.Transform(_transform);	// Transform the point to non transformed cicle space

		auto& lastCircle = _circles.back();
		const auto distanceFromPoint = lastCircle.GetCenter().GetDistance(actualPoint);

		bool inverted = false;
		std::vector<SvgCircleEquation> circles;	//copy
		if (distanceFromPoint > lastCircle.GetRadius())
		{
			const int ratio = distanceFromPoint / lastCircle.GetRadius();
			const auto rXToAdd = lastCircle.GetRadius() * SvgFloat(ratio);

			inverted = ratio % 2 != 0;
			circles = inverted ? _inverseCircles : _circles;

			for (auto& circle : circles)
			{
				circle.ScaleBy(rXToAdd);
			}
		}
		else
		{
			circles = _circles;
		}

		const SvgCalculatedGradientStop *stopA = nullptr;
		const SvgCalculatedGradientStop *stopB = nullptr;

		const SvgCircleEquation *circleA = nullptr;
		const SvgCircleEquation *circleB = nullptr;

		if (inverted)
		{
			auto size = _stops.size() - 1;
			for (std::size_t i = size; i > 0 ; i--)
			{
				stopA = &_stops[i];
				circleA = &circles[i];
				circleB = &circles[i - 1];

				if (circles[i - 1].IsPointInside(actualPoint))
				{
					stopB = &_stops[i - 1];
					break;
				}
			}
		}
		else
		{
			auto size = _stops.size() - 1;
			for (std::size_t i = 0; i < size; i++)
			{
				stopA = &_stops[i];
				circleA = &circles[i];
				circleB = &circles[i + 1];

				if (circles[i + 1].IsPointInside(actualPoint))
				{
					stopB = &_stops[i + 1];
					break;
				}
			}
		}

		assert(stopA != nullptr);
		assert(stopB != nullptr);

		return CalculateColorFromStops(actualPoint, *stopA, *stopB, *circleA, *circleB);
	}

	void SvgCalculatedRadialGradient::Transform(const SvgTransform &transform)
	{
		// since this will move the points further we need to perform the inverse transform to move them back to the circle space.
		SvgTransform actualTransform;
		actualTransform.Inverse(transform);

		_transform.Multiply(actualTransform);
	}
} // Elpida
