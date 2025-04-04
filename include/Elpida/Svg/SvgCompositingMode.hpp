//
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVGCOMPOSITINGOPERATOR_HPP
#define ELPIDA_SVGCOMPOSITINGOPERATOR_HPP
#include <string_view>

namespace Elpida
{
	enum class SvgCompositingMode
	{
		Clear,
		Copy,
		Destination,
		SourceOver,
		DestinationOver,
		SourceIn,
		DestinationIn,
		SourceOut,
		DestinationOut,
		SourceAtop,
		DestinationAtop,
		Xor,
		Lighter
	};

	class SvgCompositingModeParser
	{
	public:
		static SvgCompositingMode Parse(std::string_view view);
	};
} // Elpida

#endif //ELPIDA_SVGCOMPOSITINGOPERATOR_HPP
