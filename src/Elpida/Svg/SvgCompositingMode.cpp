//
// Created by klapeto on 24/3/2024.
//

#include "Elpida/Svg/SvgCompositingMode.hpp"

namespace Elpida
{
	SvgCompositingMode SvgCompositingModeParser::Parse(std::string_view view)
	{
		return SvgCompositingMode::SourceOver;
	}
} // Elpida