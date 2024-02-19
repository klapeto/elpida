//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGBOUNDS_HPP
#define ELPIDA_SVG_SVGBOUNDS_HPP

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
			return _minX;
		}

		[[nodiscard]]
		double GetMinY() const
		{
			return _minY;
		}

		[[nodiscard]]
		double GetMaxX() const
		{
			return _maxX;
		}

		[[nodiscard]]
		double GetMaxY() const
		{
			return _maxY;
		}

		[[nodiscard]]
		double GetWidth() const
		{
			return _maxX - _minX;
		}

		[[nodiscard]]
		double GetHeight() const
		{
			return _maxY - _minY;
		}

		void Merge(const SvgBounds& other);

		SvgBounds() : _minX(0.0),
		              _minY(0.0),
		              _maxX(0.0),
		              _maxY(0.0)
		{
		}

		SvgBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c, const SvgPoint& d);

		SvgBounds(const double minX, const double minY, const double maxX, const double maxY)
			: _minX(minX),
			  _minY(minY),
			  _maxX(maxX),
			  _maxY(maxY)
		{
		}

	private:
		double _minX;
		double _minY;
		double _maxX;
		double _maxY;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGBOUNDS_HPP
