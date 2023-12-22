//
// Created by klapeto on 22/12/2023.
//

#ifndef ELPIDA_SVG_SVGPATHCOMMAND_HPP
#define ELPIDA_SVG_SVGPATHCOMMAND_HPP

#include <vector>

namespace Elpida
{
	enum class SvgPathCommandType
	{
		MoveTo,
		LineTo,
		HorizontalLineTo,
		VerticalLineTo,
		CurveTo,
		SmoothCurveTo,
		QuadraticCurveTo,
		SmoothQuadraticCurveTo,
		Arc,
		ClosePath,
	};

	class SvgPathCommand
	{
	public:
		union CommandData
		{
			struct MoveToData
			{
				double x;
				double y;
			} moveToData;

			struct LineToData
			{
				double x;
				double y;
			} lineToData;

			struct HorizontalLineToData
			{
				double x;
			} horizontalLineToData;

			struct VerticalLineToData
			{
				double y;
			} verticalLineToData;

			struct CurveToData
			{
				double x1;
				double y1;
				double x2;
				double y2;
				double x;
				double y;
			} curveToData;

			struct SmoothCurveToData
			{
				double x2;
				double y2;
				double x;
				double y;
			} smoothCurveToData;

			struct QuadraticCurveToData
			{
				double x1;
				double y1;
				double x;
				double y;
			} quadraticCurveToData;

			struct SmoothQuadraticCurveToData
			{
				double x;
				double y;
			} smoothQuadraticCurveToData;

			struct ArcData
			{
				double rX;
				double rY;
				double xRotation;
				bool largeArc;
				bool sweep;
				double x;
				double y;
			} arcData;
		};

		SvgPathCommand(const SvgPathCommandType type, std::vector<CommandData>&& data, const bool absolute)
			: _data(std::move(data)), _type(type), _absolute(absolute)
		{
		}

	private:
		std::vector<CommandData> _data;
		SvgPathCommandType _type;
		bool _absolute;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPATHCOMMAND_HPP
