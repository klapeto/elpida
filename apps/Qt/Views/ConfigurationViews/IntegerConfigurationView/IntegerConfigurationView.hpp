#ifndef ELPIDA_APPLICATION_INTEGERCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_INTEGERCONFIGURATIONVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	namespace Ui
	{
		class IntegerConfigurationView;
	}

	class IntegerConfigurationView : public ConfigurationView
	{
	 Q_OBJECT

	 public:
		explicit IntegerConfigurationView(QWidget* parent = nullptr);
		~IntegerConfigurationView() override;
	 protected:
		void OnModelSet() override;
	 private slots:
		void on_spnValue_valueChanged(int value);
	 private:
		Ui::IntegerConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_INTEGERCONFIGURATIONVIEW_HPP
