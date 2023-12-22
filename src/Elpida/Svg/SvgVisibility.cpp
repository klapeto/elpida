//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgVisibility.hpp"

namespace Elpida
{
	SvgVisibility::SvgVisibility(const std::string_view view)
	{
		_visible = view != "none";
	}
} // Elpida
