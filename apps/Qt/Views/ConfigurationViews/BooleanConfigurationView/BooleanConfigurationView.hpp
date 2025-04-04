#ifndef ELPIDA_APPLICATION_BOOLEANCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_BOOLEANCONFIGURATIONVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	namespace Ui
	{
		class BooleanConfigurationView;
	}

	class BooleanConfigurationView : public ConfigurationView
	{
	Q_OBJECT

	public:
		explicit BooleanConfigurationView(QWidget* parent = nullptr);
		~BooleanConfigurationView() override;
	protected:
		void OnModelSet() override;
	private slots:
		void on_chkConfig_stateChanged(int state);
	private:
		Ui::BooleanConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_BOOLEANCONFIGURATIONVIEW_HPP
