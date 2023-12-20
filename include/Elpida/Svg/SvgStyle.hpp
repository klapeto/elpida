//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGSTYLE_HPP
#define ELPIDA_SVG_SVGSTYLE_HPP

#include <string>
#include <unordered_map>

namespace Elpida
{
	class SvgStyle
	{
	public:
		[[nodiscard]]
		const std::unordered_map<std::string, std::string>& GetRules() const
		{
			return _rules;
		}

		template<typename T>
		T As(const std::string& name) const
		{
			if (const auto itr = _rules.find(name); itr != _rules.end())
			{
				return T(itr->second);
			}
			return T();
		}

		SvgStyle() = default;
		explicit SvgStyle(std::string_view view);
	private:
		std::unordered_map<std::string, std::string> _rules;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSTYLE_HPP
