//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVGSVGVISIBILITY_HPP
#define ELPIDA_SVGSVGVISIBILITY_HPP

#include <string_view>

namespace Elpida
{
	class SvgVisibility
	{
	public:
		operator bool() const
		{
			return _visible;
		}

		constexpr SvgVisibility()
			: _visible(true)
		{
		}

		explicit SvgVisibility(std::string_view view);

	private:
		bool _visible;
	};
} // Elpida

#endif //ELPIDA_SVGSVGVISIBILITY_HPP
