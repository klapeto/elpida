/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#include "TopologyNodeFrame.hpp"
#include "ui_TopologyNodeFrame.h"

namespace Elpida
{

	TopologyNodeFrame::TopologyNodeFrame(const ProcessorNode& node)
		:
		QFrame(nullptr),
		_node(node),
		ui(new Ui::TopologyNodeFrame),
		_checkBox(nullptr),
		_mouseDown(false),
		_mouseOver(false)
	{
		ui->setupUi(this);
	}

	TopologyNodeFrame::~TopologyNodeFrame()
	{
		delete ui;
	}

	void TopologyNodeFrame::mousePressEvent(QMouseEvent* event)
	{
		_mouseDown = true;
		setStyleSheet(_clickedStyle);
	}

	void TopologyNodeFrame::mouseReleaseEvent(QMouseEvent* event)
	{
		_mouseDown = false;
		setStyleSheet(_mouseOverStyle);
		emit clicked(this);
	}

	void TopologyNodeFrame::enterEvent(QEvent* event)
	{
		_mouseOver = true;
		setStyleSheet(_mouseOverStyle);
	}

	void TopologyNodeFrame::leaveEvent(QEvent* event)
	{
		_mouseOver = false;
		setStyleSheet(_defaultStyle);
	}

	void TopologyNodeFrame::setDefaultStyle(const QString& defaultStyle)
	{
		_defaultStyle = defaultStyle;
		if (!_mouseDown && !_mouseOver)
		{
			setStyleSheet(_defaultStyle);
		}
	}

	void TopologyNodeFrame::setClickedStyle(const QString& clickedStyle)
	{
		_clickedStyle = clickedStyle;
		if (_mouseDown)
		{
			setStyleSheet(_clickedStyle);
		}
	}

	void TopologyNodeFrame::setMouseOverStyle(const QString& mouseOverStyle)
	{
		_mouseOverStyle = mouseOverStyle;
		if (_mouseOver && !_mouseDown)
		{
			setStyleSheet(_mouseOverStyle);
		}
	}

} // namespace Elpida
