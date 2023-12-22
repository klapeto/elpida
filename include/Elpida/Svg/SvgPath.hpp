//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGPATH_HPP
#define ELPIDA_SVG_SVGPATH_HPP

#include "SvgPathCommand.hpp"
#include "SvgShape.hpp"

namespace Elpida
{

	class SvgPath : public SvgShape
	{
	public:
		explicit SvgPath(const XmlElement& element, SvgDefs& defs);
	private:
		std::vector<SvgPathCommand> _commands;
		bool _closed;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPATH_HPP
