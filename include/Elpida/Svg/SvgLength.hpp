//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGCOORDINATE_HPP
#define ELPIDA_SVG_SVGCOORDINATE_HPP

#include <string_view>

namespace Elpida
{
	enum class SvgUnits
	{
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
		constexpr double CalculatePixels(double orig, double length, double fontSize, double dpi) const
		{
			switch (_units)
			{
			case SvgUnits::Px:
				return _value;
			case SvgUnits::Pt:
				return _value / 72.0f * dpi;
			case SvgUnits::Pc:
				return _value / 6.0f * dpi;
			case SvgUnits::Mm:
				return _value / 25.4f * dpi;
			case SvgUnits::Cm:
				return _value / 2.54f * dpi;
			case SvgUnits::In:
				return _value * dpi;
			case SvgUnits::Percent:
				return orig + (_value / 100.0f * length);
			case SvgUnits::Em:
				return _value * fontSize;
			case SvgUnits::Ex:
				return _value * fontSize * 0.52f;
			default:
				return _value;
			}
		}

		constexpr SvgLength()
			: _value(0), _units(SvgUnits::Px)
		{
		}
		constexpr explicit SvgLength(double value, SvgUnits units)
			: _value(value), _units(units)
		{
		}

		explicit SvgLength(std::string_view view, bool ignoreErrors = true);

		~SvgLength() = default;
	private:
		double _value;
		SvgUnits _units;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGCOORDINATE_HPP
