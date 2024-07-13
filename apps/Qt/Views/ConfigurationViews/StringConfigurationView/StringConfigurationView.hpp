#ifndef ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	namespace Ui
	{
		class StringConfigurationView;
	}

	class StringConfigurationView : public ConfigurationView
	{
	 Q_OBJECT

	 public:
		explicit StringConfigurationView(QWidget* parent = nullptr);
		~StringConfigurationView() override;
	 protected:
		void OnModelSet() override;
	 private slots:
		void on_leValue_textChanged(const QString& value);
	 private:
		Ui::StringConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP
