#include "NumberInputView.hpp"
#include "ui_NumberInputView.h"

#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>
#include <Elpida/Engine/Configuration/ConfigurationSpecificationBase.hpp>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Config.hpp>

#include <QComboBox>

namespace Elpida
{

	NumberInputView::NumberInputView()
		: ConfigurationValueViewBase(), _ui(new Ui::NumberInputView), _configurationValue(nullptr)
	{
		_ui->setupUi(this);
		_ui->dsbNumber->setMaximum(1024);
		_ui->cbStandard->addItem("IEC");
		_ui->cbStandard->addItem("SI");

		reInitializePrefix();

		QWidget::connect(_ui->cbStandard,
			QOverload<int>::of(&QComboBox::currentIndexChanged),
			this,
			&NumberInputView::onStandardChanged);
		QWidget::connect(_ui->cbUnitScale,
			QOverload<int>::of(&QComboBox::currentIndexChanged),
			this,
			&NumberInputView::onScaleChanged);
		QWidget::connect(_ui->dsbNumber,
			QOverload<double>::of(&QDoubleSpinBox::valueChanged),
			this,
			&NumberInputView::onValueChanged);
	}

	NumberInputView::~NumberInputView()
	{
		delete _ui;
	}

	void NumberInputView::setConfiguration(ConfigurationValueBase* configurationValue)
	{
		if (configurationValue != nullptr)
		{
			auto& spec = configurationValue->getConfigurationSpecification();
			switch (spec.getType())
			{
			case ConfigurationType::Type::Int:
				_ui->dsbNumber->setDecimals(0);
				_ui->dsbNumber->setMinimum(-99);
				_ui->dsbNumber
					->setValue(configurationValue->as<ConfigurationValue<ConfigurationType::Int>>().getValue());
				break;
			case ConfigurationType::Type::UnsignedInt:
				_ui->dsbNumber->setDecimals(0);
				_ui->dsbNumber->setMinimum(0);
				_ui->dsbNumber
					->setValue(configurationValue->as<ConfigurationValue<ConfigurationType::UnsignedInt>>().getValue());
				break;
			case ConfigurationType::Type::Float:
				_ui->dsbNumber->setMinimum(-99.9999);
				_ui->dsbNumber
					->setValue(configurationValue->as<ConfigurationValue<ConfigurationType::Float>>().getValue());
				break;
			default:
				throw ElpidaException(FUNCTION_NAME, "Invalid Configuration! Expected number configuration");
				break;
			}
			_ui->lblPropertyName->setText(QString::fromStdString(spec.getName()));
		}
		_configurationValue = configurationValue;
	}

	ConfigurationValueBase* NumberInputView::getConfiguration()
	{
		return _configurationValue;
	}

	void NumberInputView::saveSetting()
	{
		if (_configurationValue != nullptr)
		{
			auto value = getValue();
			auto& spec = _configurationValue->getConfigurationSpecification();

			switch (spec.getType())
			{
			case ConfigurationType::Type::Int:
				_configurationValue->as<ConfigurationValue<ConfigurationType::Int>>().setValue(value);
				break;
			case ConfigurationType::Type::UnsignedInt:
				_configurationValue->as<ConfigurationValue<ConfigurationType::UnsignedInt>>()
					.setValue(value);
				break;
			case ConfigurationType::Type::Float:
				_configurationValue->as<ConfigurationValue<ConfigurationType::Float>>()
					.setValue(value);
				break;
			default:
				throw ElpidaException(FUNCTION_NAME, "Invalid Configuration! Expected number configuration");
				break;
			}
		}
	}

	double NumberInputView::getValue() const
	{
		auto index = _ui->cbUnitScale->currentIndex();
		if ((unsigned int)index < ((sizeof Vu::ScaleValuesSI) / (sizeof Vu::ScaleValuesSI[0])) &&
			(unsigned int)index < ((sizeof Vu::ScaleValuesIEC) / (sizeof Vu::ScaleValuesIEC[0])))
		{
			auto scale = _ui->cbStandard->currentIndex() == 0 ? Vu::ScaleValuesIEC[index] : Vu::ScaleValuesSI[index];
			return _ui->dsbNumber->value() * scale;
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Out of range Scale enumeration!");
		}
	}

	void NumberInputView::reInitializePrefix()
	{
		_ui->cbUnitScale->clear();

		if (_ui->cbStandard->currentIndex() == 0)
		{
			for (auto scl : Vu::PrefixesIEC)
			{
				_ui->cbUnitScale->addItem(scl);
			}
		}
		else
		{
			for (auto scl : Vu::PrefixesSI)
			{
				_ui->cbUnitScale->addItem(scl);
			}
		}
		saveSetting();
	}

	void NumberInputView::onStandardChanged(int state)
	{
		reInitializePrefix();
	}

	void NumberInputView::onValueChanged(double value)
	{
		saveSetting();
	}

	void NumberInputView::onScaleChanged(int state)
	{
		saveSetting();
	}

} // namespace Elpida
