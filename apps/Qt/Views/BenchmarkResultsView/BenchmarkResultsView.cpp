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

#include "BenchmarkResultsView.hpp"
#include "ui_BenchmarkResultsView.h"

#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/BenchmarkScoreCalculator.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>

#include <QTreeWidgetItem>

namespace Elpida
{

	BenchmarkResultsView::BenchmarkResultsView(const CollectionModel<BenchmarkResult>& model)
		:
		QWidget(), CollectionModelObserver<BenchmarkResult>(model), _ui(new Ui::BenchmarkResultsView)
	{
		_ui->setupUi(this);
	}

	BenchmarkResultsView::~BenchmarkResultsView()
	{
		delete _ui;
	}

	static std::string getResultString(const TaskResult& result)
	{
		auto& metrics = result.getMetrics();
		auto& resultSpec = result.getTaskSpecification().getResultSpecification();
		if (resultSpec.getType() == ResultType::Throughput)
		{
			return Vu::Cs(Vu::getValueScaleStringSI(
				metrics.getResultValue() / metrics.getDurationSubdivision<Second>()),
				resultSpec.getUnit(),
				"/s");
		}
		else
		{
			return Vu::Cs(Vu::getValueScaleStringSI(metrics.getResultValue()), resultSpec.getUnit());
		}
	}

	static Duration getTotalDuration(const std::vector<TaskResult>& results)
	{
		auto accumulator = 0.0;
		for (auto& result: results)
		{
			accumulator += result.getMetrics().getDuration().count();
		}
		return Duration(accumulator);
	}

	static std::string getScoreString(const std::vector<TaskResult>& results,
		const BenchmarkScoreCalculator& calculator)
	{
		if (calculator.getResultType() == ResultType::Throughput)
		{
			return Vu::Cs(Vu::getValueScaleStringSI(
				calculator.calculate(results) / (getTotalDuration(results).count() * Second::den) / Second::num),
				calculator.getSuffix(),
				"/s");
		}
		else
		{
			return Vu::Cs(Vu::getValueScaleStringSI(calculator.calculate(results)), calculator.getSuffix());
		}
	}

	void BenchmarkResultsView::onItemAdded(const BenchmarkResult& item)
	{
		auto parent = new QTreeWidgetItem();
		parent->setText(0, QString::fromStdString(item.getBenchmark().getName()));
		auto& results = item.getTaskResults();
		for (const auto& result: results)
		{
			auto child = new QTreeWidgetItem();

			auto name = result.getTaskSpecification().getName();

			if (result.getTaskSpecification().acceptsInput())
			{
				name = Vu::Cs(name,
					" [",
					Vu::getValueScaleStringSI(result.getMetrics().getInputDataSize()),
					result.getTaskSpecification().getInputDataSpecification().getUnit(),
					"]");
			}

			child->setText(0, QString::fromStdString(name));
			child->setText(1,
				QString::fromStdString(getResultString(result)));
			parent->addChild(child);
		}
		auto& scoreCalculator = item.getBenchmark().getScoreCalculator();
		parent->setText(1, QString::fromStdString(getScoreString(results, scoreCalculator)));
		_createdItems.emplace(item.getId(), parent);
		_ui->twResultList->addTopLevelItem(parent);
	}

	void BenchmarkResultsView::onItemRemoved(const BenchmarkResult& item)
	{
		auto itr = _createdItems.find(item.getId());
		if (itr != _createdItems.end())
		{
			_ui->twResultList->removeItemWidget(itr->second, 0);
			_createdItems.erase(itr);
		}
	}

	void BenchmarkResultsView::onCollectionCleared()
	{
		_ui->twResultList->clear();
		_createdItems.clear();
	}

} // namespace Elpida
