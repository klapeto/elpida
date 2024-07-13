#ifndef ELPIDA_APPLICATION_FILECONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_FILECONFIGURATIONVIEW_HPP

#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	namespace Ui
	{
		class FileConfigurationView;
	}

	class FileConfigurationView : public ConfigurationView
	{
	 Q_OBJECT

	 public:

		explicit FileConfigurationView(QWidget* parent = nullptr);
		~FileConfigurationView() override;
	 protected:
		void OnModelSet() override;
	 private slots:
		void on_pbOpen_clicked(bool checked);
	 	void on_leValue_textChanged(const QString& value);
	 private:
		Ui::FileConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_FILECONFIGURATIONVIEW_HPP
