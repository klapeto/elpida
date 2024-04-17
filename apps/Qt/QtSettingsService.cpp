//
// Created by klapeto on 11/9/2023.
//

#include "QtSettingsService.hpp"

namespace Elpida::Application
{
	void QtSettingsService::Set(const std::string& name, const std::string& value)
	{
		_settings.setValue(QString::fromStdString(name), QString::fromStdString(value));
	}

	std::string QtSettingsService::Get(const std::string& name)
	{
		return _settings.value(QString::fromStdString(name)).toString().toStdString();
	}
} // Application