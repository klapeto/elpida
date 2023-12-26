//
// Created by klapeto on 23/12/2023.
//

#include "Elpida/Svg/SvgRect.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgNumber.hpp>

namespace Elpida
{
	SvgRect::SvgRect(const XmlElement& element, SvgDocument& document)
		: SvgPath(element, document)
	{
		auto& properties = GetProperties();
		auto& viewBox = document.GetElement().GetViewBox();
		_x = SvgLength(properties.GetValue("x")).CalculateActualValue(viewBox.GetMinX(), viewBox.GetWidth(),
		                                                              document.GetFontSize(), document.GetDpi());
		_y = SvgLength(properties.GetValue("y")).CalculateActualValue(viewBox.GetMinY(), viewBox.GetHeight(),
		                                                              document.GetFontSize(), document.GetDpi());

		_width = SvgLength(properties.GetValue("width")).CalculateActualValue(
			0, viewBox.GetWidth(), document.GetFontSize(), document.GetDpi());
		if (_width < 0.0)
		{
			_width = 0.0;
		}

		_height = SvgLength(properties.GetValue("height")).CalculateActualValue(
			0, viewBox.GetHeight(), document.GetFontSize(), document.GetDpi());
		if (_height < 0.0)
		{
			_height = 0.0;
		}

		auto& rx = properties.GetValue("rx");
		auto& ry = properties.GetValue("ry");

		_rX = SvgLength(rx).CalculateActualValue(0, viewBox.GetWidth(), document.GetFontSize(), document.GetDpi());;
		_rY = SvgLength(ry).CalculateActualValue(0, viewBox.GetHeight(), document.GetFontSize(), document.GetDpi());;

		if (!rx.empty() && ry.empty())
		{
			_rY = _rX;
		}

		if (rx.empty() && !ry.empty())
		{
			_rX = _rY;
		}

		if (_rX > _width / 2.0f) _rX = _width / 2.0f;
		if (_rY > _height / 2.0f) _rY = _height / 2.0f;

		if (_width > 0 && _height > 0)
		{
			_commands.push_back(SvgPathCommand(SvgPathCommandType::MoveTo,
			                                   {
				                                   SvgPathCommand::CommandData{.moveToData{.x = _x + _rX, .y = _y}}
			                                   }, true));

			_commands.push_back(SvgPathCommand(SvgPathCommandType::HorizontalLineTo, {
				                                   SvgPathCommand::CommandData{
					                                   .horizontalLineToData = {.x = _x + _width - _rX}
				                                   }
			                                   }, true));
			if (_rX > 0 && _rY > 0)
			{
				_commands.push_back(SvgPathCommand(SvgPathCommandType::Arc, {
									   SvgPathCommand::CommandData{
										   .arcData = {
										   	.x = _x + _width,
										   	.y = _y + _rY,
										   	.rX = _rX,
										   	.rY = _rY,
										   	.xRotation = 0,
										   	.largeArc = false,
										   	.sweep = true
										   }
									   }
								   }, true));
			}

			_commands.push_back(SvgPathCommand(SvgPathCommandType::VerticalLineTo, {
									   SvgPathCommand::CommandData{
										   .verticalLineToData = {.y = _y + _height - _rY}
									   }
								   }, true));

			if (_rX > 0 && _rY > 0)
			{
				_commands.push_back(SvgPathCommand(SvgPathCommandType::Arc, {
									   SvgPathCommand::CommandData{
										   .arcData = {
											   .x = _x + _width - _rX,
											   .y = _y + _height,
											   .rX = _rX,
											   .rY = _rY,
											   .xRotation = 0,
											   .largeArc = false,
											   .sweep = true
										   }
									   }
								   }, true));
			}

			_commands.push_back(SvgPathCommand(SvgPathCommandType::HorizontalLineTo, {
									   SvgPathCommand::CommandData{
										   .horizontalLineToData = {.x = _x + _rX}
									   }
								   }, true));

			if (_rX > 0 && _rY > 0)
			{
				_commands.push_back(SvgPathCommand(SvgPathCommandType::Arc, {
									   SvgPathCommand::CommandData{
										   .arcData = {
											   .x = _x,
											   .y = _y + _height - _rY,
											   .rX = _rX,
											   .rY = _rY,
											   .xRotation = 0,
											   .largeArc = false,
											   .sweep = true
										   }
									   }
								   }, true));
			}

			_commands.push_back(SvgPathCommand(SvgPathCommandType::VerticalLineTo, {
									   SvgPathCommand::CommandData{
										   .verticalLineToData = {.y = _y + _rY}
									   }
								   }, true));

			if (_rX > 0 && _rY > 0)
			{
				_commands.push_back(SvgPathCommand(SvgPathCommandType::Arc, {
									   SvgPathCommand::CommandData{
										   .arcData = {
											   .x = _x,
											   .y = _y + _height - _rY,
											   .rX = _rX,
											   .rY = _rY,
											   .xRotation = 0,
											   .largeArc = false,
											   .sweep = true
										   }
									   }
								   }, true));
			}

			_closed = true;
		}
	}
} // Elpida
