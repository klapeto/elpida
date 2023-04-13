#ifndef ELPIDA_APPLICATION_FILECONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_FILECONFIGURATIONVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{

	namespace Ui
	{
		class FileConfigurationView;
	}

	class FileConfigurationView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit FileConfigurationView(QWidget* parent = nullptr);
		~FileConfigurationView() override;

	 private:
		Ui::FileConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_FILECONFIGURATIONVIEW_HPP
