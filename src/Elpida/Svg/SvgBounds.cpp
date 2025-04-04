//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgBounds.hpp"

#include "Elpida/Svg/SvgPoint.hpp"

namespace Elpida
{
	void SvgBounds::Merge(const SvgBounds& other)
	{
		_min.ToMin(other._min);
		_max.ToMax(other._max);
	}

	SvgBounds::SvgBounds(const SvgPoint& a)
			:_min(a), _max(a)
	{
	}

	SvgBounds::SvgBounds(const SvgPoint& a, const SvgPoint& b)
			:SvgBounds(a)
	{
		_min.ToMin(b);
		_max.ToMax(b);
	}

	bool SvgBounds::Contains(const SvgPoint& point) const
	{
		return point.GetX() >= _min.GetX() && point.GetX() <= _max.GetX() && point.GetY() >= _min.GetY() &&
			   point.GetY() <= _max.GetY();
	}

	SvgBounds::SvgBounds(const SvgFloat minX, const SvgFloat minY, const SvgFloat maxX, const SvgFloat maxY)
			:_min(minX, minY), _max(maxX, maxY)
	{
	}

	SvgBounds SvgBounds::CreateMinimum()
	{
		return {
				std::numeric_limits<SvgFloat>::max(),
				std::numeric_limits<SvgFloat>::max(),
				std::numeric_limits<SvgFloat>::lowest(),
				std::numeric_limits<SvgFloat>::lowest()
		};
	}
} // Elpida
