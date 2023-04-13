#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarksModel.hpp"
#include <QLabel>

namespace Elpida::Application
{
	BenchmarkConfigurationView::BenchmarkConfigurationView()
		: QWidget(),
		  _ui(new Ui::BenchmarkConfigurationView)
	{
		_ui->setupUi(this);
		_ui->gbConfiguration->setLayout(new QVBoxLayout);
	}

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		delete _ui;
	}

	void BenchmarkConfigurationView::SetModel(BenchmarkModel* benchmarkModel)
	{
		_benchmarkModel = benchmarkModel;

		auto layout = _ui->gbConfiguration->layout();

		for (auto widget : _createdWidgets)
		{
			layout->removeWidget(widget);
			widget->setParent(nullptr);
			delete widget;
		}

		_createdWidgets.clear();

		if (_benchmarkModel != nullptr)
		{

			for (auto& config: _benchmarkModel->GetConfigurations())
			{
				auto w = new QLabel(QString::fromStdString(config.GetName()));
				_createdWidgets.push_back(w);
				layout->addWidget(w);
			}
		}
	}
}

