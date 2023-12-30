//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SVG_SVGDOCUMENT_HPP

#include <stack>

#include "SvgDefs.hpp"
#include "SvgFillRule.hpp"
#include "SvgSvgElement.hpp"
#include "SvgLineJoin.hpp"
#include "SvgLineCap.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgDocument
	{
	public:
		[[nodiscard]]
		const SvgDefs& GetDefs() const
		{
			return _defs;
		}

		[[nodiscard]]
		const SvgSvgElement& GetElement() const
		{
			return _element;
		}

		constexpr double GetDpi() const
		{
			return _dpi;
		}

		double GetFontSize() const
		{
			return _fontSizes.top();
		}

		double GetActualWidth() const
		{
			return _element.GetViewBox().GetWidth();
		}

		double GetActualHeight() const
		{
			return _element.GetViewBox().GetHeight();
		}

		double GetActualLength() const
		{
			const double width = GetActualWidth();
			const double height = GetActualHeight();
			return Sqrt(width * width + height * height) / Sqrt(2.0);
		}

		explicit SvgDocument(const XmlElement& element);
		SvgDocument(SvgDocument&& other) noexcept = default;

	private:
		SvgDefs _defs;
		SvgSvgElement _element;
		double _dpi = 96.0;
		std::stack<double> _fontSizes;
		// std::stack<SvgLineCap> _lineCaps;	// for inheritance
		// std::stack<SvgLineJoin> _lineJoins;
		// std::stack<SvgFillRule> _fillRules;

		static constexpr double Sqrt(const double x)
		{
			return sqrt(x);
		}

		friend class SvgElement;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGDOCUMENT_HPP
