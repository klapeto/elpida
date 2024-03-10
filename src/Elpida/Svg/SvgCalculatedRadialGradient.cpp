//
// Created by klapeto on 10/3/2024.
//

#include "Elpida/Svg/SvgCalculatedRadialGradient.hpp"

#include "Elpida/Svg/SvgGradient.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgPoint.hpp"

#include <cassert>

namespace Elpida
{
	SvgCalculatedRadialGradient::SvgCalculatedRadialGradient(const SvgGradient& gradient,
			const SvgBounds& elementBounds,
			std::vector<SvgCalculatedGradientStop>&& stops,
			const SvgCalculationContext& calculationContext)
			:SvgCalculatedGradient(std::move(stops), gradient.GetSpreadType())
	{
		const auto bounds = gradient.GetUnits() == SvgGradientUnits::User
							? SvgBounds(0.0, 0.0, 1.0, 1.0)
							: elementBounds;
		auto& data = gradient.GetData().radial;

		const auto actualRadius = data.r.CalculateValue(calculationContext, bounds.GetWidth());
		auto actualCX = data.cx.CalculateValue(calculationContext, bounds.GetWidth());
		auto actualCY = data.cy.CalculateValue(calculationContext, bounds.GetHeight());

		_equations.reserve(_stops.size());

		for (const auto& stop : _stops)
		{
			// calculate the linear interpolation of the radius
			const auto thisRadius = std::lerp(0, actualRadius, stop.GetOffset());

			_equations.emplace_back(actualCX, actualCY, thisRadius, thisRadius)
					.Transform(gradient.GetGradientTransform());
		}
	}

	SvgColor SvgCalculatedRadialGradient::CalculateColor(const SvgPoint& point) const
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

	SvgColor SvgCalculatedRadialGradient::CalculatePad(const SvgPoint& point) const
	{
		const SvgCalculatedGradientStop* stopA = nullptr;
		const SvgCalculatedGradientStop* stopB = nullptr;

		const SvgEllipseEquation* equationA = nullptr;
		const SvgEllipseEquation* equationB = nullptr;

		for (std::size_t i = 0; i < _stops.size() - 1; i++)
		{
			stopA = &_stops[i];
			equationA = &_equations[i];
			equationB = &_equations[i + 1];

			if (_equations[i + 1].IsPointInside(point))
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

	SvgColor SvgCalculatedRadialGradient::CalculateRepeat(const SvgPoint& point) const
	{
		auto stopEquations = _equations; // Copy

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

		assert(stopA != nullptr);
		assert(stopB != nullptr);

		auto closestPointToA = equationA->CalculateClosestPoint(point);
		auto closestPointToB = equationB->CalculateClosestPoint(point);

		double distanceFromB = closestPointToB.GetDistance(point);

		auto ratio = closestPointToA == point && closestPointToA != equationA->GetCenter()
					 ? 1.0
					 : distanceFromB / closestPointToA.GetDistance(closestPointToB);

		return InterpolateColor(*stopA, *stopB, ratio);
	}

	SvgColor SvgCalculatedRadialGradient::CalculateReflect(const SvgPoint& point) const
	{
		std::vector<SvgEllipseEquation> stopEquations;
		std::vector<std::reference_wrapper<const SvgCalculatedGradientStop>> stops;

		stops.reserve(_equations.size());
		stopEquations.reserve(_equations.size());

		const auto& lastStopEquation = _equations.back();
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
				auto size = _equations.size();

				for (std::size_t i = 0; i < size; ++i)
				{
					stopEquations.push_back(_equations[size - i - 1]);
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
				for (std::size_t i = 0; i < _equations.size(); ++i)
				{
					stopEquations.push_back(_equations[i]);
					stopEquations.back().Expand(rXToAdd, ryToAdd, lastStopEquation.GetAngle());
					stops.emplace_back(_stops[i]);
				}
			}
		}
		else
		{
			for (std::size_t i = 0; i < _equations.size(); ++i)
			{
				stopEquations.push_back(_equations[i]);
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

		assert(stopA != nullptr);
		assert(stopB != nullptr);

		auto closestPointToA = equationA->CalculateClosestPoint(point);
		auto closestPointToB = equationB->CalculateClosestPoint(point);

		double distanceFromB = closestPointToB.GetDistance(point);

		auto ratio = closestPointToA == point && closestPointToA != equationA->GetCenter()
					 ? 1.0
					 : distanceFromB / closestPointToA.GetDistance(closestPointToB);

		return InterpolateColor(*stopA, *stopB, ratio);
	}

	void SvgCalculatedRadialGradient::Transform(const SvgTransform& transform)
	{
		for (auto& equation : _equations)
		{
			equation.Transform(transform);
		}
	}
} // Elpida