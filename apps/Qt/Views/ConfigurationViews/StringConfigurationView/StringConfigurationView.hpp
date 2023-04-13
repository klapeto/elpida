#ifndef ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{

	namespace Ui
	{
		class StringConfigurationView;
	}

	class StringConfigurationView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit StringConfigurationView(QWidget* parent = nullptr);
		~StringConfigurationView() override;

	 private:
		Ui::StringConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_STRINGCONFIGURATIONVIEW_HPP
