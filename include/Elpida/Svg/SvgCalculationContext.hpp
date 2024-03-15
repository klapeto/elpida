//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGCALCULATIONCONTEXT_HPP
#define ELPIDA_SVGCALCULATIONCONTEXT_HPP

#include "Elpida/Svg/SvgViewBox.hpp"
#include "Elpida/Svg/SvgCalculatedViewPort.hpp"

#include <stack>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

namespace Elpida
{
	class XmlMap;
	class SvgElement;

	class SvgCalculationContext
	{
	public:

		[[nodiscard]]
		const SvgViewBox& GetViewBox() const;

		[[nodiscard]]
		const SvgCalculatedViewPort& GetViewPort() const;

		[[nodiscard]]
		std::string_view GetValue(const std::string_view& name) const;

		[[nodiscard]]
		double GetNumber(const std::string_view& name) const;

		[[nodiscard]]
		SvgTransform CalculateTransform() const;

		[[nodiscard]]
		double GetDpi() const;

		[[nodiscard]]
		double GetRootFontSize() const;

		void Push(const SvgElement& element);

		void Pop();

		SvgCalculationContext(double rootFontSize, double dpi);
	private:
		std::stack<SvgViewBox> _viewBox;
		std::stack<SvgCalculatedViewPort> _viewPort;
		std::vector<SvgTransform> _transforms;
		std::unordered_map<std::string_view, std::stack<std::string>> _stackedValues;
		double _rootFontSize;
		double _dpi;
		std::size_t _currentDepth;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATIONCONTEXT_HPP
