/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#include "NavigationBarView.hpp"
#include "ui_NavigationBarView.h"

#include "Models/Screens/ScreensModel.hpp"

#include <QtWidgets/QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/QButtonGroup>
#include <QSpacerItem>

namespace Elpida
{

	NavigationBarView::NavigationBarView(ScreensModel& model)
		: QWidget(), _ui(new Ui::NavigationBarView), _model(model)
	{
		_ui->setupUi(this);

		_layout = new QHBoxLayout();
		_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
		setLayout(_layout);

		_buttonGroup = new QButtonGroup();
		_buttonGroup->setExclusive(true);

		addSubscription(model.itemAdded.subscribe([this](const CollectionChangedEventArgs<ScreenItem>& item)
		{
			emit onViewAdded(const_cast<ScreenItem&>(item.getItem().getValue()));
		}));

		QObject::connect(_buttonGroup,
			QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled),
			this,
			&NavigationBarView::onButton_Toggled);

		QObject::connect(this, &NavigationBarView::onViewAdded, this, &NavigationBarView::onViewAdded_handler);
	}

	NavigationBarView::~NavigationBarView()
	{
		delete _buttonGroup;
		delete _ui;
	}

	void NavigationBarView::onViewAdded_handler(ScreenItem& screen)
	{
		auto button = new QPushButton(QString::fromStdString(screen.getName()));

		button->setCheckable(true);

		auto index = _layout->count() - 1;

		if (index == 0)
		{
			button->setChecked(true);    // TODO: find better way to sync selected page
		}

		_screenMap.emplace(index, &screen);

		_buttonGroup->addButton(button, index);
		_layout->insertWidget(index, button);
	}

	void NavigationBarView::onButton_Toggled(QAbstractButton* button, bool checked)
	{
		_model.setSelectedScreen(_screenMap.at(_buttonGroup->id(button)));
	}

} // namespace Elpida
