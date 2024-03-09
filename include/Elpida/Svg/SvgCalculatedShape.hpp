//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGCALCULATEDSHAPE_HPP
#define ELPIDA_SVGCALCULATEDSHAPE_HPP

#include "Elpida/Svg/SvgPathInstance.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"

#include <vector>
#include <optional>

namespace Elpida
{
	class SvgPaint;
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
		const std::optional<SvgCalculatedPaint>& GetStroke() const;

		SvgCalculatedShape() = default;
		explicit SvgCalculatedShape(std::vector<SvgPathInstance>&& paths,
				const SvgPaint& fill,
				const SvgPaint& stroke,
				const SvgDocument& document,
				const SvgCalculationContext& calculationContext);
	private:
		std::vector<SvgPathInstance> _paths;
		std::vector<SvgCalculatedShape> _children;
		std::optional<SvgCalculatedPaint> _fill;
		std::optional<SvgCalculatedPaint> _stroke;
		SvgBounds _bounds;

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
