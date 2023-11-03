//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGRADIALDATA_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGRADIALDATA_HPP

#include "SvgCoordinate.hpp"
namespace Elpida
{

	class SvgRadialData
	{
	public:
		const SvgCoordinate& GetCx() const
		{
			return _cx;
		}
		const SvgCoordinate& GetCy() const
		{
			return _cy;
		}
		const SvgCoordinate& GetR() const
		{
			return _r;
		}
		const SvgCoordinate& GetFx() const
		{
			return _fx;
		}
		const SvgCoordinate& GetFy() const
		{
			return _fy;
		}

		SvgRadialData();
	private:
		SvgCoordinate _cx;
		SvgCoordinate _cy;
		SvgCoordinate _r;
		SvgCoordinate _fx;
		SvgCoordinate _fy;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGRADIALDATA_HPP
