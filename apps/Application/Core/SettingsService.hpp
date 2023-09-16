//
// Created by klapeto on 11/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_CORE_SETTINGSSERVICE_HPP
#define ELPIDA_APPS_APPLICATION_CORE_SETTINGSSERVICE_HPP

#include <string>

namespace Elpida::Application
{
	class SettingsService
	{
	public:
		virtual void Set(const std::string& name, std::string& value) = 0;
		virtual std::string Get(const std::string& name) = 0;
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_CORE_SETTINGSSERVICE_HPP
