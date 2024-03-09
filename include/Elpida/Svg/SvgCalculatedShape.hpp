//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDSHAPE_HPP
#define ELPIDA_SVGCALCULATEDSHAPE_HPP

#include "Elpida/Svg/SvgPathInstance.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "SvgCalculatedStroke.hpp"

#include <vector>
#include <optional>

namespace Elpida
{
	class SvgPaint;
	class SvgStroke;

	class SvgCalculatedShape
	{
	public:

		[[nodiscard]]
		const std::vector<SvgPathInstance>& GetPaths() const;

		[[nodiscard]]
		const std::vector<SvgCalculatedShape>& GetChildren() const;

		[[nodiscard]]
		const SvgBounds& GetBounds() const;

		[[nodiscard]]
		const std::optional<SvgCalculatedPaint>& GetFill() const;

		[[nodiscard]]
		const std::optional<SvgCalculatedStroke>& GetStroke() const;

		[[nodiscard]]
		double GetOpacity() const;

		SvgCalculatedShape();
		explicit SvgCalculatedShape(std::vector<SvgPathInstance>&& paths,
				const SvgPaint& fill,
				const SvgStroke& stroke,
				const SvgDocument& document,
				double opacity,
				const SvgCalculationContext& calculationContext);
	private:
		std::vector<SvgPathInstance> _paths;
		std::vector<SvgCalculatedShape> _children;
		std::optional<SvgCalculatedPaint> _fill;
		std::optional<SvgCalculatedStroke> _stroke;
		SvgBounds _bounds;
		double _opacity;

		void RecalculateBounds();

		void AddChildren(std::vector<SvgCalculatedShape>&& children)
		{
			_children = std::move(children);
			RecalculateBounds();
		}

		friend class SvgElement;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDSHAPE_HPP
