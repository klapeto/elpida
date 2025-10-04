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

#ifndef ELPIDA_TOPOLOGYNODEVIEW_HPP
#define ELPIDA_TOPOLOGYNODEVIEW_HPP

#include <QFrame>
#include <optional>

#include "Models/SystemInfo/TopologyNodeModel.hpp"
#include "EventSubscription.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class TopologyNodeView;
	}

	class TopologyNodeModel;

	class TopologyNodeView : public QFrame
	{
	Q_OBJECT
	public:
		explicit TopologyNodeView(TopologyNodeModel& topologyNodeModel, std::optional<unsigned int>& fastestProcessor, QWidget* parent = nullptr);
		~TopologyNodeView() override;
	protected:
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void enterEvent(QEvent* event) override;
		void leaveEvent(QEvent* event) override;
	private slots:
		void on_chkSelected_stateChanged(int state);
	private:
		Ui::TopologyNodeView* _ui;
		TopologyNodeModel& _topologyNodeModel;
		EventSubscription<> _dataChangedSubscription;
		bool _uiUpdating;
		bool _fastest;
		void OnModelChanged();
	};
}

#endif // ELPIDA_TOPOLOGYNODEVIEW_HPP
