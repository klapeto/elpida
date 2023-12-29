//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOORDINATE_HPP
#define ELPIDA_SVG_SVGCOORDINATE_HPP

#include <string_view>

namespace Elpida
{
	class SvgDocument;
}

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

	class SvgLength final
	{
	public:

		[[nodiscard]]
		constexpr double GetValue() const
		{
			return _value;
		}

		[[nodiscard]]
		constexpr SvgUnits GetUnits() const
		{
			return _units;
		}

		[[nodiscard]]
		double CalculateActualValue(const SvgDocument& document, double orig, double length) const;

		constexpr SvgLength()
			: _value(0), _units(SvgUnits::Raw)
		{
		}
		constexpr explicit SvgLength(const double value, const SvgUnits units)
			: _value(value), _units(units)
		{
		}

		explicit SvgLength(std::string_view view);

		~SvgLength() = default;
	private:
		double _value;
		SvgUnits _units;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGCOORDINATE_HPP
