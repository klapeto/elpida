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
			return std::abs(_max.GetX()) - std::abs(_min.GetX());
		}

		[[nodiscard]]
		double GetHeight() const
		{
			return std::abs( _max.GetY()) - std::abs(_min.GetY());
		}

		[[nodiscard]]
		bool IsValid() const
		{
			return _max.GetX() >= _min.GetX()
				   && _max.GetY() >= _min.GetY()
				   && GetWidth() > 0
				   && GetHeight() > 0;
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
