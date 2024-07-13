//
// Created by klapeto on 28/12/2023.
//

#ifndef ELPIDA_SVG_SVGPAINT_HPP
#define ELPIDA_SVG_SVGPAINT_HPP

#include <string>

#include "SvgColor.hpp"

namespace Elpida
{
	class XmlMap;

	class SvgPaint
	{
	public:
		[[nodiscard]]
		bool IsSet() const
		{
			return _set;
		}

		[[nodiscard]]
		const std::string& GetGradientId() const
		{
			return _gradientId;
		}

		[[nodiscard]]
		const SvgColor& GetColor() const
		{
			return _color;
		}

		[[nodiscard]]
		double GetOpacity() const
		{
			return _opacity;
		}

		SvgPaint()
			: _opacity(1.0), _set(false)
		{
		}

	protected:
		std::string _gradientId;
		SvgColor _color;
		double _opacity;

		bool _set;

		void ParseColor(const std::string& value);
		void ParseOpacity(const std::string& value);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPAINT_HPP
