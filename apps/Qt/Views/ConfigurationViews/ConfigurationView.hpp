//
// Created by klapeto on 14/4/2023.
//

#ifndef ELPIDA_CONFIGURATIONVIEW_HPP_
#define ELPIDA_CONFIGURATIONVIEW_HPP_

#include <QWidget>

namespace Elpida::Application
{

	class BenchmarkConfigurationInstanceModel;
	class ConfigurationView : public QWidget
	{
	 public:
		void SetModel(BenchmarkConfigurationInstanceModel* model);
		explicit ConfigurationView(QWidget* parent = nullptr);
		~ConfigurationView() override = default;
	 protected:
		BenchmarkConfigurationInstanceModel* _model;
		virtual void OnModelSet() = 0;
	};

} // Application

#endif //ELPIDA_CONFIGURATIONVIEW_HPP_
