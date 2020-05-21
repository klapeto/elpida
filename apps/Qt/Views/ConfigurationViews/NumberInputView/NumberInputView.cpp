#include "NumberInputView.hpp"
#include "ui_NumberInputView.h"

#include <Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp>
#include <Elpida/Engine/Configuration/Specification/ConfigurationSpecificationBase.hpp>
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
				assignValueToUi(configurationValue->as<ConfigurationValue<ConfigurationType::Int>>().getValue());
				break;
			case ConfigurationType::Type::UnsignedInt:
				_ui->dsbNumber->setMinimum(0);
				assignValueToUi(configurationValue->as<ConfigurationValue<ConfigurationType::UnsignedInt>>()
					.getValue());
				break;
			case ConfigurationType::Type::Float:
				assignValueToUi(configurationValue->as<ConfigurationValue<ConfigurationType::Float>>().getValue());
				break;
			default:
				throw ElpidaException(FUNCTION_NAME, "Invalid Configuration! Expected number configuration");
			}
			_ui->lblPropertyName->setText(QString::fromStdString(spec.getName()));
		}
		_configurationValue = configurationValue;
	}

	struct ScaleResults
	{
		size_t index;
		double denominator;
	};

	static ScaleResults getResult(double value, const double scaleValues[], size_t arraySize)
	{
		auto index = 0u;
		double denominator = 0;
		for (; index < arraySize; ++index)
		{
			denominator = scaleValues[index];
			if (value / denominator <= denominator)
			{
				break;
			}
		}
		return { index, denominator };
	}

	void NumberInputView::assignValueToUi(double value) const
	{
		auto results =
			_ui->cbStandard->currentIndex() == 0 ?
			getResult(value, Vu::ScaleValuesIEC, Vu::getArrayLength(Vu::ScaleValuesIEC))
												 : getResult(value,
				Vu::ScaleValuesSI,
				Vu::getArrayLength(Vu::ScaleValuesSI));

		_ui->dsbNumber->setMaximum(_ui->cbStandard->currentIndex() == 0 ? 1023 : 1000);
		_ui->cbUnitScale->setCurrentIndex(results.index);
		_ui->dsbNumber->setValue(value / results.denominator);
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
			}
		}
	}

	double NumberInputView::getValue() const
	{
		auto index = _ui->cbUnitScale->currentIndex();
		return _ui->cbStandard->currentIndex() == 0 ?
			   getScaledValue(index, Vu::ScaleValuesIEC, Vu::getArrayLength(Vu::ScaleValuesIEC))
													: getScaledValue(index,
				Vu::ScaleValuesSI,
				Vu::getArrayLength(Vu::ScaleValuesSI));
	}

	double NumberInputView::getScaledValue(int index, const double scaleValues[], size_t size) const
	{
		if ((unsigned int)index < size)
		{
			return _ui->dsbNumber->value() * scaleValues[index];;
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
