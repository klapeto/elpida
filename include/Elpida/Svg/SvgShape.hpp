//
// Created by klapeto on 21/12/2023.
//

#ifndef ELPIDA_SVG_SVGSHAPE_HPP
#define ELPIDA_SVG_SVGSHAPE_HPP

#include <optional>

#include "SvgElement.hpp"
#include "SvgFill.hpp"
#include "SvgStroke.hpp"

namespace Elpida
{
	class SvgShape : public SvgElement
	{
	public:
		[[nodiscard]] const std::optional<SvgFill>& GetFill() const
		{
			return _fill;
		}

		[[nodiscard]] const std::optional<SvgStroke>& GetStroke() const
		{
			return _stroke;
		}

		[[nodiscard]] double GetOpacity() const
		{
			return _opacity;
		}

		[[nodiscard]] bool IsVisible() const
		{
			return _visible;
		}

		explicit SvgShape(const XmlElement& element, SvgDocument& document);
	private:
		std::optional<SvgFill> _fill;
		std::optional<SvgStroke> _stroke;
		double _opacity;
		bool _visible;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSHAPE_HPP
