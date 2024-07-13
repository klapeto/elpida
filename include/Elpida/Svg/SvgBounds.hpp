//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGBOUNDS_HPP
#define ELPIDA_SVG_SVGBOUNDS_HPP

#include "Elpida/Svg/SvgPoint.hpp"

namespace Elpida
{
	class SvgPoint;

	class SvgBounds
	{
	public:
		[[nodiscard]]
		bool Contains(const SvgPoint& point) const;

		[[nodiscard]]
		double GetMinX() const
		{
			return _min.GetX();
		}

		[[nodiscard]]
		double GetMinY() const
		{
			return _min.GetY();
		}

		[[nodiscard]]
		double GetMaxX() const
		{
			return _max.GetX();
		}

		[[nodiscard]]
		double GetMaxY() const
		{
			return _max.GetY();
		}

		[[nodiscard]]
		double GetWidth() const
		{
			return _max.GetX() - _min.GetX();
		}

		[[nodiscard]]
		double GetHeight() const
		{
			return _max.GetY() - _min.GetY();
		}

		void Merge(const SvgBounds& other);

		static SvgBounds CreateMinimum();

		SvgBounds() = default;
		explicit SvgBounds(const SvgPoint& a);
		SvgBounds(const SvgPoint& a, const SvgPoint& b);
		SvgBounds(double minX, double minY, double maxX, double maxY);
		~SvgBounds() = default;
	private:
		SvgPoint _min;
		SvgPoint _max;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGBOUNDS_HPP
