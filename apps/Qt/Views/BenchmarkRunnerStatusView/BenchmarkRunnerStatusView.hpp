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

#ifndef ELPIDA_TASKBATCHRUNNERSTATUS_HPP
#define ELPIDA_TASKBATCHRUNNERSTATUS_HPP

#include <QWidget>
#include <Elpida/Utilities/OptionalReference.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

namespace Elpida
{

	class EventSubscriptionBase;
	class BenchmarkRunnerModel;

	namespace Ui
	{
		class BenchmarkRunnerStatusView;
	}

	class BenchmarkRunnerStatusView : public QWidget
	{
	Q_OBJECT

	public:
		explicit BenchmarkRunnerStatusView(const BenchmarkRunnerModel& model);
		~BenchmarkRunnerStatusView() override;
	private:
		Ui::BenchmarkRunnerStatusView* _ui;
		const BenchmarkRunnerModel& _model;
		std::vector<Reference<EventSubscriptionBase>> _subscriptions;

		QString _runningString;
		QString _readyString;
		QString _naString;
		QString _zeroString;

		signals:
		void onCurrentRunningTaskChanged();
		void onCurrentRunningBenchmarkChanged();
		void onCompletedBenchmarkTasksChanged();
		void onCompletedTaskIterationsChanged();
		void onCompletedBenchmarksChanged();
		void onTotalBenchmarksChanged();
		void onRunningChanged();

	private slots:
		void updateCurrentRunningTaskChanged();
		void updateCurrentRunningBenchmarkChanged();
		void updateCompletedBenchmarkTasksChanged();
		void updateCompletedTaskIterationsChanged();
		void updateCompletedBenchmarksChanged();
		void updateTotalBenchmarksChanged();
		void updateRunningChanged();
	};

} // namespace Elpida

#endif // ELPIDA_TASKBATCHRUNNERSTATUS_HPP
