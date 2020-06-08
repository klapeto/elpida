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

#ifndef ELPIDA_NAVIGATIONBARVIEW_HPP
#define ELPIDA_NAVIGATIONBARVIEW_HPP

#include <QWidget>
#include <unordered_map>
#include <Elpida/EventsSubscriber.hpp>

class QHBoxLayout;
class QButtonGroup;
class QAbstractButton;

namespace Elpida
{

	class ScreenItem;
	class ScreensModel;

	namespace Ui
	{
		class NavigationBarView;
	}

	class NavigationBarView : public QWidget, public EventsSubscriber
	{
	Q_OBJECT

	public:
		explicit NavigationBarView(ScreensModel& model);
		~NavigationBarView() override;

	signals:
		void onViewAdded(ScreenItem& screen);

	private:
		std::unordered_map<int, ScreenItem*> _screenMap;
		Ui::NavigationBarView* _ui;
		ScreensModel& _model;
		QHBoxLayout* _layout;
		QButtonGroup* _buttonGroup;

	private slots:
		void onViewAdded_handler(ScreenItem& screen);
		void onButton_Toggled(QAbstractButton* button, bool checked);
	};

} // namespace Elpida

#endif // ELPIDA_NAVIGATIONBARVIEW_HPP
