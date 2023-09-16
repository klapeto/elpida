//
// Created by klapeto on 11/9/2023.
//

#ifndef ELPIDA_APPS_QT_QTSETTINGSSERVICE_HPP
#define ELPIDA_APPS_QT_QTSETTINGSSERVICE_HPP

#include <QSettings>
#include "Core/SettingsService.hpp"
namespace Elpida::Application
{

	class QtSettingsService final: public SettingsService
	{
	public:
		void Set(const std::string& name, std::string& value) override;
		std::string Get(const std::string& name) override;

		QtSettingsService() = default;
		QtSettingsService(const QtSettingsService&) = delete;
		QtSettingsService(QtSettingsService&&) noexcept= delete;
		QtSettingsService operator=(const QtSettingsService&) = delete;
		QtSettingsService operator=(const QtSettingsService&&) noexcept = delete;
		~QtSettingsService() = default;
	private:
		QSettings settings;
	};

} // Application

#endif //ELPIDA_APPS_QT_QTSETTINGSSERVICE_HPP
