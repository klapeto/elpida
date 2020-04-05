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

#include "TopologyFrame.hpp"
#include "ui_TopologyFrame.h"

namespace Elpida
{

	TopologyFrame::TopologyFrame(const ProcessorNode& node)
		:
		QFrame(nullptr),
		_node(node),
		ui(new Ui::TopologyFrame),
		_checkBox(nullptr),
		_clickAble(true),
		_mouseDown(false),
		_mouseOver(false)
	{
		ui->setupUi(this);
	}

	TopologyFrame::~TopologyFrame()
	{
		delete ui;
	}

	void TopologyFrame::mousePressEvent(QMouseEvent* event)
	{
		if (_clickAble)
		{
			_mouseDown = true;
			setStyleSheet(_clickedStyle);
		}
	}

	void TopologyFrame::mouseReleaseEvent(QMouseEvent* event)
	{
		if (_clickAble)
		{
			_mouseDown = false;
			setStyleSheet(_mouseOverStyle);
			emit clicked(this);
		}
	}

	void TopologyFrame::enterEvent(QEvent* event)
	{
		if (_clickAble)
		{
			_mouseOver = true;
			setStyleSheet(_mouseOverStyle);
		}
	}

	void TopologyFrame::leaveEvent(QEvent* event)
	{
		if (_clickAble)
		{
			_mouseOver = false;
			setStyleSheet(_defaultStyle);
		}
	}

} // namespace Elpida
