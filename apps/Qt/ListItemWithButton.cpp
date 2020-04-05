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

#include "ListItemWithButton.hpp"
#include "ui_ListItemWithButton.h"

#include <iostream>

namespace Elpida
{

	ListItemWithButton::ListItemWithButton(const QString& text, QWidget *parent)
			: QWidget(parent), _ui(new Ui::ListItemWithButton)
	{
		_ui->setupUi(this);
		_ui->lblName->setText(text);
		_ui->pbButton->setVisible(false);
	}

	ListItemWithButton::~ListItemWithButton()
	{
		delete _ui;
	}

	void ListItemWithButton::enterEvent(QEvent* event)
	{
		_ui->pbButton->setVisible(true);
	}

	void ListItemWithButton::setText(const QString& text)
	{
		_ui->lblName->setText(text);
	}

	void ListItemWithButton::leaveEvent(QEvent* event)
	{
		_ui->pbButton->setVisible(false);
	}

	void ListItemWithButton::setButtonText(const QString& text)
	{
		_ui->pbButton->setText(text);
	}

	void ListItemWithButton::setButtonIcon(const QIcon& icon)
	{
		_ui->pbButton->setIcon(icon);
	}

	QString ListItemWithButton::getText() const
	{
		return _ui->lblName->text();
	}

	void ListItemWithButton::on_pbButton_clicked()
	{
		emit buttonClicked(_ui->lblName->text());
	}

} // namespace Elpida
