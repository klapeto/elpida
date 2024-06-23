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
			SvgBackDrop& tempBackdrop,
			const SvgSuperSampler& superSampler)
	{
		tempBackdrop.Clear();

		bool doDraw = false;
		auto& fill = shape.GetFill();
		if (fill.has_value() && !shape.GetFillPolygon().GetEdges().empty())
		{
			doDraw = true;
			tempBackdrop.Draw(shape.GetFillPolygon(), fill.value(), superSampler, fill->GetFillRule());
		}

		auto& stroke = shape.GetStroke();
		if (stroke.has_value() && !shape.GetStrokePolygon().GetEdges().empty())
		{
			doDraw = true;
			tempBackdrop.Draw(shape.GetStrokePolygon(), stroke.value(), superSampler, SvgFillRule::NonZero);
		}

		if (doDraw)
		{
			auto& bounds = shape.GetBounds();
			finalBackdrop.Draw(tempBackdrop,
					std::max(0.0, std::floor(bounds.GetMinX())),
					std::max(0.0, std::floor(bounds.GetMinY())),
					std::ceil(bounds.GetWidth() + 1),
					std::ceil(bounds.GetHeight() + 1),
					shape.GetOpacity());
		}

		for (auto& child : shape.GetChildren())
		{
			RasterizeShape(child, finalBackdrop, tempBackdrop, superSampler);
		}
	}

	void SvgDirectRasterizer::Rasterize(const SvgCalculatedDocument& document,
			SvgBackDrop& backDrop,
			SvgBackDrop& tempBackDrop,
			std::size_t subSamples)
	{
		const SvgSuperSampler superSampler(subSamples);
		backDrop.Clear();
		RasterizeShape(document.GetRootShape(), backDrop, tempBackDrop, superSampler);
	}
} // Elpida