//
// Created by klapeto on 23/6/2024.
//

#include "Elpida/Svg/SvgDirectRasterizer.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{

	static void RasterizeShape(const SvgCalculatedShape& shape,
			SvgBackDrop& finalBackdrop,
			const SvgSuperSampler& superSampler)
	{
		auto& fill = shape.GetFill();
		if (fill.has_value() && !shape.GetFillPolygon().GetEdges().empty())
		{
			finalBackdrop.Draw(shape.GetFillPolygon(), fill.value(), superSampler, fill->GetFillRule(), shape.BlendMode(), shape.CompositingMode(), shape.GetOpacity());
		}

		auto& stroke = shape.GetStroke();
		if (stroke.has_value() && !shape.GetStrokePolygon().GetEdges().empty())
		{
			finalBackdrop.Draw(shape.GetStrokePolygon(), stroke.value(), superSampler, SvgFillRule::NonZero, shape.BlendMode(), shape.CompositingMode(), shape.GetOpacity());
		}

		for (auto& child : shape.GetChildren())
		{
			RasterizeShape(child, finalBackdrop, superSampler);
		}
	}

	void SvgDirectRasterizer::Rasterize(const SvgCalculatedDocument& document,
			SvgBackDrop& backDrop,
			std::size_t subSamples)
	{
		const SvgSuperSampler superSampler(subSamples);
		backDrop.Clear();
		RasterizeShape(document.GetRootShape(), backDrop, superSampler);
	}
} // Elpida