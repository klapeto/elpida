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
		: ConfigurationValueViewBase(), _ui(new Ui::NumberInputView), _configurationValue(nullptr), _currentRawValue(0.0),
		  _updating(false)
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
				setValue(configurationValue->as<ConfigurationValue<ConfigurationType::Int>>().getValue());
				break;
			case ConfigurationType::Type::UnsignedInt:
				_ui->dsbNumber->setMinimum(0);
				setValue(configurationValue->as<ConfigurationValue<ConfigurationType::UnsignedInt>>()
					.getValue());
				break;
			case ConfigurationType::Type::Float:
				setValue(configurationValue->as<ConfigurationValue<ConfigurationType::Float>>().getValue());
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

	static ScaleResults getScaledResult(double value, const double* scaleValues, size_t arraySize)
	{
		size_t i = arraySize - 1;
		while (i > 0)
		{
			if (value >= scaleValues[i])
			{
				break;
			}
			i--;
		}

		return { i, scaleValues[i] };
	}

	void NumberInputView::setValue(double value)
	{
		_updating = true;
		auto results =
			_ui->cbStandard->currentIndex() == 0 ?
			getScaledResult(value, Vu::ScaleValuesIEC, Vu::getArrayLength(Vu::ScaleValuesIEC))
			: getScaledResult(value, Vu::ScaleValuesSI, Vu::getArrayLength(Vu::ScaleValuesSI));

		_currentRawValue = value;

		_ui->dsbNumber->setMaximum(_ui->cbStandard->currentIndex() == 0 ? 1023 : 1000);
		_ui->cbUnitScale->setCurrentIndex(results.index);
		_ui->dsbNumber->setValue(value / results.denominator);
		_updating = false;
	}

	ConfigurationValueBase* NumberInputView::getConfiguration()
	{
		return _configurationValue;
	}

	void NumberInputView::saveSetting()
	{
		if (_configurationValue != nullptr)
		{
			auto currentValue = getScaledValue();
			auto& spec = _configurationValue->getConfigurationSpecification();

			switch (spec.getType())
			{
			case ConfigurationType::Type::Int:
				_configurationValue->as<ConfigurationValue<ConfigurationType::Int>>().setValue(currentValue);
				break;
			case ConfigurationType::Type::UnsignedInt:
				_configurationValue->as<ConfigurationValue<ConfigurationType::UnsignedInt>>()
					.setValue(currentValue);
				break;
			case ConfigurationType::Type::Float:
				_configurationValue->as<ConfigurationValue<ConfigurationType::Float>>()
					.setValue(currentValue);
				break;
			default:
				throw ElpidaException(FUNCTION_NAME, "Invalid Configuration! Expected number configuration");
			}
		}
	}

	double NumberInputView::getScaledValue() const
	{
		auto index = _ui->cbUnitScale->currentIndex();

		return _ui->cbStandard->currentIndex() == 0 ?
			   calculateScaledValue(index, Vu::ScaleValuesIEC, Vu::getArrayLength(Vu::ScaleValuesIEC))
			   : calculateScaledValue(index, Vu::ScaleValuesSI, Vu::getArrayLength(Vu::ScaleValuesSI));
	}

	double NumberInputView::calculateScaledValue(int index, const double* scaleValues, size_t size) const
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
		_updating = true;
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
		setValue(_currentRawValue);
		saveSetting();
		_updating = false;
	}

	void NumberInputView::onStandardChanged(int state)
	{
		reInitializePrefix();
	}

	void NumberInputView::onValueChanged(double value)
	{
		if (!_updating)
		{
			saveSetting();
		}
	}

	void NumberInputView::onScaleChanged(int state)
	{
		if (!_updating)
		{
			setValue(getScaledValue());
			saveSetting();
		}
	}

} // namespace Elpida
