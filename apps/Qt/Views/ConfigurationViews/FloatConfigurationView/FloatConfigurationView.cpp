#include "FloatConfigurationView.hpp"
#include "ui_FloatConfigurationView.h"

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

namespace Elpida::Application
{

	FloatConfigurationView::FloatConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::FloatConfigurationView)
	{
		_ui->setupUi(this);
	}

	FloatConfigurationView::~FloatConfigurationView()
	{
		delete _ui;
	}

	void FloatConfigurationView::on_spnValue_valueChanged(double value)
	{
		auto toString = [](double d)
		{
			std::ostringstream doubleToStringAccumulator;
			doubleToStringAccumulator.imbue(std::locale::classic());
			doubleToStringAccumulator << d;
			return doubleToStringAccumulator.str();
		};

		if (_model != nullptr && !_settingModel)
		{
			_controller->SetValue(toString(value));
		}
	}

	void FloatConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
			_ui->spnValue->setValue(ValueUtilities::StringToDoubleInvariant(_model->GetValue()));
		}
	}

} // namespace Elpida
