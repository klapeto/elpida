//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgPath.hpp"
#include "Elpida/Svg/SvgPathGenerator.hpp"
#include "Elpida/Xml/XmlMap.hpp"

namespace Elpida
{
	SvgPath::SvgPath(const XmlMap& properties)
	{
		_pathData = properties.GetValue("d");
	}

	std::vector<SvgPathInstance> SvgPath::CalculatePaths() const
	{
		SvgPathGenerator generator;
		generator.ParsePathData(_pathData);
		return std::move(generator.GetPaths());
	}
} // Elpida