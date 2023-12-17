//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTSTOP_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTSTOP_HPP

#include "SvgColor.hpp"
namespace Elpida
{

	class SvgGradientStop
	{
	public:
		const SvgColor& GetColor() const
		{
			return _color;
		}

		double GetOffset() const
		{
			return _offset;
		}

	private:
		SvgColor _color;
		double _offset;
		double _opacity;
		bool _percent;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENTSTOP_HPP
