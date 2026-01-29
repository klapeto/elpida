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
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOORDINATE_HPP
#define ELPIDA_SVG_SVGCOORDINATE_HPP

#include <string_view>
#include "SvgConfig.hpp"

namespace Elpida
{
	enum class SvgUnits
	{
		Raw,
		Px,
		Pt,
		Pc,
		Mm,
		Cm,
		In,
		Percent,
		Em,
		Ex
	};

	class SvgDocument;
	class SvgCalculationContext;

	class SvgLength final
	{
	public:

		[[nodiscard]]
		constexpr SvgFloat GetValue() const
		{
			return _value;
		}

		[[nodiscard]]
		constexpr SvgUnits GetUnits() const
		{
			return _units;
		}

		[[nodiscard]]
		SvgFloat CalculateValue(const SvgCalculationContext& calculationContext, SvgFloat orig, SvgFloat length) const;

		[[nodiscard]]
		SvgFloat CalculateValue(const SvgCalculationContext& calculationContext, SvgFloat length) const
		{
			return CalculateValue(calculationContext, 0, length);
		}

		constexpr SvgLength()
			: _value(0), _units(SvgUnits::Raw)
		{
		}
		constexpr explicit SvgLength(const SvgFloat value, const SvgUnits units)
			: _value(value), _units(units)
		{
		}

		explicit SvgLength(std::string_view view);

		~SvgLength() = default;
	private:
		SvgFloat _value;
		SvgUnits _units;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGCOORDINATE_HPP
