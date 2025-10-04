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

#ifndef ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
#define ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include <QWidget>

namespace Elpida::Application
{

	class CustomBenchmarkModel;

	namespace Ui
	{
		class CustomBenchmarkResultsView;
	}

	class CustomBenchmarkResultsView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit CustomBenchmarkResultsView(const CustomBenchmarkModel& model, QWidget* parent = nullptr);
		~CustomBenchmarkResultsView() override;

	 private:
		Ui::CustomBenchmarkResultsView* _ui;
		const CustomBenchmarkModel& _model;
		EventSubscription<const CollectionItem<BenchmarkResultModel>&> _resultAdded;
		EventSubscription<> _resultsCleared;

		void OnResultsCleared();
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
