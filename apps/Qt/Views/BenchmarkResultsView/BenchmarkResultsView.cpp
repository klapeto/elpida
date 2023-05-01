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
#include <Elpida/Engine/Calculators/BenchmarkScoreCalculator.hpp>
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

	static std::string getResultString(const ProcessedTaskResult& result)
	{
		auto& metrics = result.getFinalMetrics();
		auto& resultSpec = result.getTaskSpecification().getResultSpecification();
		if (resultSpec.getType() == ResultType::Throughput)
		{
			auto seconds = DurationCast<Seconds>(metrics.getDuration()).count();
			auto value = metrics.getResultValue();

			auto str = Vu::getValueScaleStringSI(value / seconds);

			return Vu::Cs(str, resultSpec.getUnit(), "/s");
		}
		else
		{
			return Vu::Cs(Vu::getValueScaleStringSI(metrics.getResultValue()), resultSpec.getUnit());
		}
	}

	static Duration getTotalDuration(const std::vector<ProcessedTaskResult>& results)
	{
		auto accumulator = 0.0;
		for (auto& result: results)
		{
			accumulator += result.getFinalMetrics().getDuration().count();
		}
		return Duration(accumulator);
	}

	static std::string getScoreString(const BenchmarkScore& score,
		const Benchmark& benchmark)
	{
		return Vu::Cs(Vu::getValueScaleStringSI(score.getValue()), benchmark.getScoreSpecification().getUnit());

	}

	void BenchmarkResultsView::onItemAdded(const BenchmarkResult& item)
	{
		auto parent = new QTreeWidgetItem();
		parent->setText(0, QString::fromStdString(item.getBenchmark().getName()));
		auto& results = item.getTaskResults();
		for (const auto& result: results)
		{
			auto child = new QTreeWidgetItem();
			auto& spec = result.getTaskSpecification();
			auto& name = spec.getName();
			auto& metrics = result.getFinalMetrics();

			child->setText(0, QString::fromStdString(name));
			child->setText(1, QString::fromStdString(
				Vu::Cs(Vu::getValueScaleStringSI(metrics.getInputDataSize()),
					spec.acceptsInput() ? spec.getInputDataSpecification().getUnit() : "B")));
			child->setText(2,
				QString::fromStdString(getResultString(result)));
			parent->addChild(child);
		}
		parent->setText(2, QString::fromStdString(getScoreString(item.getScore(), item.getBenchmark())));
		_createdItems.emplace(item.getId(), parent);
		_ui->twResultList->addTopLevelItem(parent);
		_ui->twResultList->setCurrentItem(parent);
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
