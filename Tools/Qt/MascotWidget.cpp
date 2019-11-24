#include "MascotWidget.hpp"
#include <QBoxLayout>
#include <QResizeEvent>

namespace Elpida
{

	MascotWidget::MascotWidget(QWidget *parent)
			: QWidget(parent)
	{

	}

	void MascotWidget::resizeEvent(QResizeEvent* event)
	{
		constexpr float arWidth = 458, arHeight = 800;
		//float thisAspectRatio = (float)event->size().width() / event->size().height();
		//int widgetStretch, outerStretch;

		auto size = event->size();
		auto minWidth = event->size().height() / (arHeight / arWidth);
		//setMinimumWidth(event->size().height() / (arHeight / arWidth));
		QWidget::resizeEvent(event);
	}

}  // namespace Elpida

