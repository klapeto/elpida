#include "FileInputView.hpp"
#include "ui_FileInputView.h"
#include <Elpida/Engine/Configuration/ConfigurationValueBase.hpp>
#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>
#include <Elpida/Engine/Configuration/ConfigurationSpecificationBase.hpp>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Config.hpp>

namespace Elpida
{

	FileInputView::FileInputView()
		: ConfigurationViewBase(), _ui(new Ui::FileInputView), _configurationValue(nullptr)
	{
		_ui->setupUi(this);
	}

	FileInputView::~FileInputView()
	{
		delete _ui;
	}

	void FileInputView::setConfiguration(ConfigurationValueBase* configurationValue)
	{
		_configurationValue = configurationValue;
		if (configurationValue != nullptr)
		{
			auto& spec = configurationValue->getConfigurationSpecification();
			if (spec.getType() == ConfigurationType::FilePath)
			{
				auto& value = _configurationValue->as<ConfigurationValue<std::string>>();
				_ui->lblPropertyName->setText(QString::fromStdString(spec.getName()));
				_ui->leFilePath->setText(QString::fromStdString(value.getValue()));
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME, "Invalid Configuration! Expected FilePath configuration");
			}

		}
	}
	ConfigurationValueBase* FileInputView::getConfiguration()
	{
		return _configurationValue;
	}

} // namespace Elpida
