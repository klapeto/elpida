//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgShape.hpp>

namespace Elpida
{
	struct Edge
	{
		double x0, y0, x1, y1;
		int dir;
	};

	static void FlatenShape(const SvgShape& shape, std::vector<Edge>& edges, double scale)
	{
	}

	void SvgRasterizer::Rasterize(const SvgDocument& document,
	                              unsigned char* outputBuffer,
	                              std::size_t bufferSize,
	                              std::size_t width,
	                              std::size_t height,
	                              std::size_t stride,
	                              double scale)
	{
		auto& viewBox = document.GetElement().GetViewBox();
		auto& defs = document.GetDefs();

		std::vector<Edge> edges;
		edges.reserve(document.GetElement().GetChildren().size() * 4);
		for (auto& element : document.GetElement().GetChildren())
		{
			if (auto shape = dynamic_cast<SvgShape*>(element.get()))
			{
				if (!shape->IsVisible()) continue;
				if (shape->GetFill().has_value())
				{
				}

				if (shape->GetStroke().has_value())
				{
				}
			}
		}
	}
} // Elpida
