//
// Created by klapeto on 22/4/20.
//

#ifndef APPS_QT_VIEWS_CONFIGURATIONVIEWS_CONFIGURATIONVIEWBASE_HPP
#define APPS_QT_VIEWS_CONFIGURATIONVIEWS_CONFIGURATIONVIEWBASE_HPP

#include <QWidget>

namespace Elpida
{
	class ConfigurationValueBase;

	class ConfigurationViewBase : public QWidget
	{
	public:
		virtual void setConfiguration(ConfigurationValueBase* configurationValue) = 0;
		virtual ConfigurationValueBase* getConfiguration() = 0;
		virtual void saveSetting() = 0;

		ConfigurationViewBase()
			: QWidget()
		{

		}
		~ConfigurationViewBase() override = default;
	};
}


#endif //APPS_QT_VIEWS_CONFIGURATIONVIEWS_CONFIGURATIONVIEWBASE_HPP
