//
// Created by klapeto on 1/4/2024.
//

#ifndef ELPIDA_SVGCALCULATEDDOCUMENT_HPP
#define ELPIDA_SVGCALCULATEDDOCUMENT_HPP

#include "Elpida/Svg/SvgCalculatedShape.hpp"
#include "Elpida/Svg/SvgViewBox.hpp"
#include "Elpida/Svg/SvgCalculatedViewPort.hpp"

namespace Elpida
{
	class SvgDocument;

	class SvgCalculatedDocument
	{
	public:
		[[nodiscard]]
		const SvgCalculatedShape& GetRootShape() const
		{
			return _rootShape;
		}


		[[nodiscard]]
		SvgCalculatedShape& GetRootShape()
		{
			return _rootShape;
		}

		[[nodiscard]]
		const SvgCalculatedViewPort& GetViewPort() const
		{
			return _viewPort;
		}

		SvgCalculatedDocument() = default;
		explicit SvgCalculatedDocument(const SvgDocument& document, double scale = 1.0);
	private:
		SvgCalculatedShape _rootShape;
		SvgCalculatedViewPort _viewPort;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDDOCUMENT_HPP
