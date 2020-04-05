#ifndef MASCOTWIDGET_HPP
#define MASCOTWIDGET_HPP

#include <QWidget>

class QBoxLayout;

namespace Elpida
{

	class MascotWidget : public QWidget
	{
	Q_OBJECT
	public:
		explicit MascotWidget(QWidget* parrent = nullptr);
	signals:

	public slots:

	protected:
		void resizeEvent(QResizeEvent* event) override;
	};

}  // namespace Elpida

#endif // MASCOTWIDGET_HPP
