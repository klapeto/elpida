#ifndef ELPIDA_APPLICATION_FCLOATCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	namespace Ui
	{
		class FloatConfigurationView;
	}


	class ConfigurationModel;
	class FloatConfigurationView : public ConfigurationView
	{
	 Q_OBJECT

	 public:

		explicit FloatConfigurationView(QWidget* parent = nullptr);
		~FloatConfigurationView() override;
	 protected:
		void OnModelSet() override;
	 private slots:
		void on_spnValue_valueChanged(double value);
	 private:
		Ui::FloatConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP
