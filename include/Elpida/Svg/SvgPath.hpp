//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGPATH_HPP
#define ELPIDA_SVGPATH_HPP

#include "SvgPathInstance.hpp"

#include <string>

namespace Elpida
{
	class SvgCalculationContext;
	class XmlMap;

	class SvgPath
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths() const;

		explicit SvgPath(const XmlMap& properties);
	private:
		std::string _pathData;
	};

} // Elpida

#endif //ELPIDA_SVGPATH_HPP
