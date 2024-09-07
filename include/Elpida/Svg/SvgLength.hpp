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
