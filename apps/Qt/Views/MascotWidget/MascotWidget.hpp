#ifndef APPS_QT_UI_MASCOTWIDGET_MASCOTWIDGET_HPP
#define APPS_QT_UI_MASCOTWIDGET_MASCOTWIDGET_HPP

#include <QWidget>

class QBoxLayout;

namespace Elpida
{

	class MascotWidget : public QWidget
	{
	Q_OBJECT
	public:
		explicit MascotWidget(QWidget* parent = nullptr);
	signals:

	public slots:

	protected:
		void resizeEvent(QResizeEvent* event) override;
	};

}  // namespace Elpida

#endif // APPS_QT_UI_MASCOTWIDGET_MASCOTWIDGET_HPP
