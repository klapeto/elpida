/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 1/4/2024.
//

#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Svg/SvgCalculatedViewPort.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"

namespace Elpida
{
	static SvgTransform CalculateTransform(const SvgCalculatedViewPort& viewPort, const SvgViewBox& viewBox,
			const SvgPreserveAspectRatio& preserveAspectRatio)
	{
		if (!viewBox.IsValid()) return {};
		auto vbX = viewBox.GetMinX();
		auto vbY = viewBox.GetMinY();
		auto vbWidth = viewBox.GetWidth();
		auto vbHeight = viewBox.GetHeight();

		auto eX = viewPort.GetX();
		auto eY = viewPort.GetY();
		auto eWidth = viewPort.GetWidth();
		auto eHeight = viewPort.GetHeight();

		auto alignX = preserveAspectRatio.GetAlignX();
		auto alignY = preserveAspectRatio.GetAlignY();
		auto meetOrSlice = preserveAspectRatio.GetType();

		auto scaleX = eWidth / vbWidth;
		auto scaleY = eHeight / vbHeight;

		if (alignX != SvgAxisAlignType::None && meetOrSlice == SvgAlignType::Meet)
		{
			if (scaleX > scaleY)
			{
				scaleX = scaleY;
			}
			else
			{
				scaleY = scaleX;
			}
		}
		else if (alignX != SvgAxisAlignType::None && meetOrSlice == SvgAlignType::Slice)
		{
			if (scaleX < scaleY)
			{
				scaleX = scaleY;
			}
			else
			{
				scaleY = scaleX;
			}
		}

		auto translateX = eX - (vbX * scaleX);
		auto translateY = eY - (vbY * scaleY);

		if (alignX == SvgAxisAlignType::Mid)
		{
			translateX += (eWidth - vbWidth * scaleX) / SvgFloat(2.0);
		}
		else if (alignX == SvgAxisAlignType::Max)
		{
			translateX += (eWidth - vbWidth * scaleX);
		}

		if (alignY == SvgAxisAlignType::Mid)
		{
			translateY += (eHeight - vbHeight * scaleY) / SvgFloat(2.0);
		}
		else if (alignY == SvgAxisAlignType::Max)
		{
			translateY += (eHeight - vbHeight * scaleY);
		}

		SvgTransform transform;
		transform.Translate(translateX, translateY)
				.Scale(scaleX, scaleY);

		return transform;
	}

	SvgCalculatedDocument::SvgCalculatedDocument(const SvgDocument& document, SvgFloat scale): _viewPort(0,0,0,0)
	{
		auto& rootSvgElement = document.GetElement();
		auto& viewBox = rootSvgElement.GetViewBox();
		auto& viewPort = rootSvgElement.GetViewPort();

		SvgCalculationContext calculationContext(1.0, 96.0);

		_viewPort = SvgCalculatedViewPort(
				0,
				0,
				viewPort.GetWidth().CalculateValue(calculationContext, 300.0),
				viewPort.GetHeight().CalculateValue(calculationContext, 150.0));

		auto calculatedViewBox = viewBox.IsValid()
								 ? viewBox
								 : SvgViewBox(0, 0, _viewPort.GetWidth(), _viewPort.GetHeight());

		_rootShape = rootSvgElement.CalculateShape(document, calculationContext);

		auto transform = CalculateTransform(_viewPort, calculatedViewBox,
				rootSvgElement.GetPreserveAspectRatio());

		transform.Scale(scale, scale);

		_rootShape.Transform(transform);
		_viewPort = SvgCalculatedViewPort(0, 0, _viewPort.GetWidth() * scale, _viewPort.GetHeight() * scale);
	}
} // Elpida