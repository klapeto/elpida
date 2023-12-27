//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGPATH_HPP
#define ELPIDA_SVG_SVGPATH_HPP

#include "SvgBounds.hpp"
#include "SvgPathCommand.hpp"
#include "SvgShape.hpp"

namespace Elpida
{

	class SvgPathPoint
	{
	public:
		[[nodiscard]]
		double GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		double GetY() const
		{
			return _y;
		}

		SvgPathPoint(const double x, const double y)
			: _x(x),
			  _y(y)
		{
		}

	private:
		double _x;
		double _y;
	};

	class SvgPathInstance
	{
	public:

	private:
		std::vector<SvgPathPoint> _points;
		SvgBounds _bounds;
		bool _closed;
	};

	class SvgPath : public SvgShape
	{
	public:
		explicit SvgPath(const XmlElement& element, SvgDocument& document);

	protected:
		std::vector<SvgPathInstance> _instances;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPATH_HPP
