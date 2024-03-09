//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGCALCULATIONCONTEXT_HPP
#define ELPIDA_SVGCALCULATIONCONTEXT_HPP

#include "SvgViewBox.hpp"
#include <stack>
#include <unordered_map>
#include <string>
#include <string_view>

namespace Elpida
{
	class XmlMap;

	class SvgCalculationContext
	{
	public:

		[[nodiscard]]
		const SvgViewBox& GetViewBox() const;

		[[nodiscard]]
		std::string_view GetValue(const std::string_view& name) const;

		[[nodiscard]]
		double GetNumber(const std::string_view& name) const;

		[[nodiscard]]
		double GetDpi() const;

		[[nodiscard]]
		double GetRootFontSize() const
		{
			return _rootFontSize;
		}

		void Push(const XmlMap& properties);

		void Pop();

		SvgCalculationContext(const SvgViewBox& viewBox, double rootFontSize, double dpi);
	private:
		SvgViewBox _viewBox;
		std::unordered_map<std::string_view, std::stack<std::string>> _stackedValues;
		double _rootFontSize;
		double _dpi;
		std::size_t _currentDepth;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATIONCONTEXT_HPP
