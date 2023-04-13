#ifndef ELPIDA_APPLICATION_INTEGERCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_INTEGERCONFIGURATIONVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{

	namespace Ui
	{
		class IntegerConfigurationView;
	}

	class IntegerConfigurationView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit IntegerConfigurationView(QWidget* parent = nullptr);
		~IntegerConfigurationView() override;
	 private:
		Ui::IntegerConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_INTEGERCONFIGURATIONVIEW_HPP
