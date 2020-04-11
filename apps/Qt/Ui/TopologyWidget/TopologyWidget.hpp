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

#ifndef APPS_QT_UI_TOPOLOGYWIDGET_TOPOLOGYWIDGET_HPP
#define APPS_QT_UI_TOPOLOGYWIDGET_TOPOLOGYWIDGET_HPP

#include <QWidget>
#include <forward_list>
#include "Elpida/TaskAffinity.hpp"

namespace Elpida
{
	class TopologyNodeFrame;
	class ProcessorNode;
	class SystemTopology;

	namespace Ui
	{
		class TopologyWidget;
	}

	class TopologyWidget : public QWidget
	{
	Q_OBJECT

	public:

		[[nodiscard]] const TaskAffinity& getAffinity() const
		{
			return _affinity;
		}

		explicit TopologyWidget(const SystemTopology& topology);
		~TopologyWidget() override;

	private:
		TaskAffinity _affinity;
		Ui::TopologyWidget* _ui;
		TopologyNodeFrame* _rootFrame;
		const SystemTopology& _topology;
		std::forward_list<const ProcessorNode*> _selectedNodes;

		void loadTopology();
		TopologyNodeFrame* getMachineWidget(const Elpida::ProcessorNode& node);
		TopologyNodeFrame* getPackageWidget(const Elpida::ProcessorNode& node);
		TopologyNodeFrame* getGroupWidget(const Elpida::ProcessorNode& node);
		static QWidget* getNumaWidget(const Elpida::ProcessorNode& node);
		TopologyNodeFrame* getCacheWidget(const Elpida::ProcessorNode& node);
		TopologyNodeFrame* getCoreWidget(const Elpida::ProcessorNode& node);
		TopologyNodeFrame* getEUWidget(const Elpida::ProcessorNode& node);
		TopologyNodeFrame* getWidget(const Elpida::ProcessorNode& node);
		TopologyNodeFrame* appendChildren(const Elpida::ProcessorNode& node);

		static void clearChildrenState(TopologyNodeFrame* frame);

		void appendAffinity(TopologyNodeFrame* frame);

	public slots:
		void onElementClick(const TopologyNodeFrame* node);
		void onClearPressed();
	};

} // namespace Elpida
#endif // APPS_QT_UI_TOPOLOGYWIDGET_TOPOLOGYWIDGET_HPP
