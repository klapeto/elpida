//
// Created by klapeto on 20/12/2023.
//

#ifndef SVGSTYLE_HPP
#define SVGSTYLE_HPP
#include <string>
#include <unordered_map>

namespace Elpida
{
	class SvgStyle
	{
	public:
		[[nodiscard]]
		const std::unordered_map<std::string, std::string>& Rules() const
		{
			return _rules;
		}

		explicit SvgStyle(std::string_view view);
	private:
		std::unordered_map<std::string, std::string> _rules;
	};
} // Elpida

#endif //SVGSTYLE_HPP
