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

#ifndef ELPIDA_TOPOLOGYWIDGET_HPP
#define ELPIDA_TOPOLOGYWIDGET_HPP

#include <QWidget>
#include <forward_list>
#include "Elpida/TaskAffinity.hpp"

namespace Elpida
{
	class TopologyFrame;
	class ProcessorNode;

	namespace Ui
	{
		class TopologyWidget;
	}

	class TopologyWidget: public QWidget
	{
		Q_OBJECT

		public:

			const TaskAffinity& getAffinity() const
			{
				return _affinity;
			}

			explicit TopologyWidget(QWidget *parent = nullptr);
			~TopologyWidget();

		private:
			TaskAffinity _affinity;
			Ui::TopologyWidget* _ui;
			TopologyFrame* _rootFrame;
			std::forward_list<const ProcessorNode*> _selectedNodes;

			void loadTopology();
			TopologyFrame* getMachineWidget(const Elpida::ProcessorNode& node);
			TopologyFrame* getPackageWidget(const Elpida::ProcessorNode& node);
			TopologyFrame* getGroupWidget(const Elpida::ProcessorNode& node);
			QWidget* getNumaWidget(const Elpida::ProcessorNode& node);
			TopologyFrame* getCacheWidget(const Elpida::ProcessorNode& node);
			TopologyFrame* getCoreWidget(const Elpida::ProcessorNode& node);
			TopologyFrame* getEUWidget(const Elpida::ProcessorNode& node);
			TopologyFrame* getWidget(const Elpida::ProcessorNode& node);
			TopologyFrame* appendChildren(const Elpida::ProcessorNode& node);

			void clearChildrenState(TopologyFrame* frame);

			void appendAffinity(TopologyFrame* frame);

		public slots:
			void onElementClick(const TopologyFrame* node);
			void onClearPressed();
	};

} // namespace Elpida
#endif // ELPIDA_TOPOLOGYWIDGET_HPP
