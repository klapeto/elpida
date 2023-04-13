#ifndef ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP
#define ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{

	namespace Ui
	{
		class FloatConfigurationView;
	}

	class FloatConfigurationView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit FloatConfigurationView(QWidget* parent = nullptr);
		~FloatConfigurationView() override;

	 private:
		Ui::FloatConfigurationView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_FLOATCONFIGURATIONVIEW_HPP
