/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGCALCULATIONCONTEXT_HPP
#define ELPIDA_SVGCALCULATIONCONTEXT_HPP

#include "Elpida/Svg/SvgViewBox.hpp"
#include "Elpida/Svg/SvgFill.hpp"
#include "Elpida/Svg/SvgStroke.hpp"
#include "Elpida/Svg/SvgCalculatedViewPort.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

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
		const SvgFill& GetFill() const;

		[[nodiscard]]
		const SvgStroke& GetStroke() const;

		[[nodiscard]]
		const SvgViewBox& GetViewBox() const;

		[[nodiscard]]
		const SvgCalculatedViewPort& GetViewPort() const;

		[[nodiscard]]
		std::string_view GetValue(const std::string_view& name) const;

		[[nodiscard]]
		SvgFloat GetNumber(const std::string_view& name) const;

		[[nodiscard]]
		SvgTransform CalculateTransform() const;

		[[nodiscard]]
		SvgFloat GetDpi() const;

		[[nodiscard]]
		SvgFloat GetOpacity() const;

		[[nodiscard]]
		SvgFloat GetRootFontSize() const;

		void Push(const SvgElement& element);

		void Pop();

		SvgCalculationContext(SvgFloat rootFontSize, SvgFloat dpi);
	private:

		template<typename T>
		class StackedValue
		{
		public:
			T value;
			std::size_t index;
		};

		std::stack<StackedValue<SvgViewBox>> _viewBox;
		std::stack<StackedValue<SvgCalculatedViewPort>> _viewPort;
		std::vector<StackedValue<SvgTransform>> _transforms;
		std::vector<StackedValue<SvgFill>> _fills;
		std::vector<StackedValue<SvgStroke>> _stokes;
		std::vector<StackedValue<SvgFloat>> _opacities;
		std::unordered_map<std::string_view, std::stack<StackedValue<std::string>>> _stackedValues;
		SvgFloat _rootFontSize;
		SvgFloat _dpi;
		std::size_t _currentDepth;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATIONCONTEXT_HPP
