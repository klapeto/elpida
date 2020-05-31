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

namespace Elpida
{

	class EventSubscriptionBase;
	class TaskSpecification;
	class Benchmark;
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
		EventSubscriptionBase* _dataChangedEventSubscription;
		const TaskSpecification* _currentRunningTaskSpecification;
		const Benchmark* _currentRunningBenchmark;

		QString _runningString;
		QString _readyString;
		QString _naString;

		bool _running;

		signals:
		void onDataUpdated();

	private slots:
		void updateUi();
	};

} // namespace Elpida

#endif // ELPIDA_TASKBATCHRUNNERSTATUS_HPP
