//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGATTRIBUTEPAIR_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGATTRIBUTEPAIR_HPP

#include <string_view>
namespace Elpida
{

	class SvgAttributePair
	{
	public:
		const std::string_view& GetName() const
		{
			return _name;
		}
		const std::string_view& GetValue() const
		{
			return _value;
		}

		SvgAttributePair(const std::string_view& name, const std::string_view& value)
			: _name(name), _value(value)
		{
		}
	private:
		std::string_view _name;
		std::string_view _value;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGATTRIBUTEPAIR_HPP
