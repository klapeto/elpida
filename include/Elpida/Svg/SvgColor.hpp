//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOLOR_HPP
#define ELPIDA_SVG_SVGCOLOR_HPP

#include <string_view>

namespace Elpida
{

	class SvgColor
	{
	public:
		unsigned int GetValue() const
		{
			return _value;
		}

		SvgColor()
			: _value(0)
		{

		}
		explicit SvgColor(std::string_view view);
	private:
		unsigned int _value;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGCOLOR_HPP
