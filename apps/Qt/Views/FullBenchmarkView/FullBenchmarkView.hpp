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

#ifndef BENCHMARKVIEW_HPP
#define BENCHMARKVIEW_HPP

#include <QWidget>
#include <memory>
#include <optional>
#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Models/Full/FullBenchmarkResultModel.hpp"
#include "Core/ThreadQueue.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class FullBenchmarkView;
	}

	class FullBenchmarkModel;
	class FullBenchmarkResultModel;
	class FullBenchmarkController;
	class BenchmarkRunConfigurationModel;
	class BenchmarkRunConfigurationController;

	class FullBenchmarkView : public QWidget
	{
	Q_OBJECT

	public:
		explicit FullBenchmarkView(
				const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
				BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
				const FullBenchmarkModel& model,
				FullBenchmarkController& controller,
				QWidget* parent = nullptr);

		~FullBenchmarkView() override;

	private:
		Ui::FullBenchmarkView* _ui;
		const FullBenchmarkModel& _model;
		const BenchmarkRunConfigurationModel& _benchmarkRunConfigurationModel;
		FullBenchmarkController& _controller;

		EventSubscription<bool> _runningChanged;
		EventSubscription<const CollectionItem<FullBenchmarkResultModel>&> _itemAdded;
		EventSubscription<std::size_t> _iterationsChanged;
		EventSubscription<std::size_t> _delayChanged;
		EventSubscription<const std::string&> _currentBenchmarkChanged;
		EventSubscription<> _resultsCleared;
		std::optional<FullBenchmarkResultModel> _previousScore;
		std::weak_ptr<ThreadQueue> _threadQueue;
		int _currentBenchmarkIndex;
		int _maxBenchmarkIndex;
		bool _cancel;

		QString _na = "N/A";

		void UpdateProgress();
		void OnResultAdded(const FullBenchmarkResultModel& result);
		void OnResultsCleared();
		void UpdateETA();

	private slots:
		void on_bpStart_clicked(bool checked);

	};
}

#endif // BENCHMARKVIEW_HPP
