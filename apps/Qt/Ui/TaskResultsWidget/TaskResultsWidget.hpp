#ifndef ELPIDA_TASKRESULTSWIDGET_HPP
#define ELPIDA_TASKRESULTSWIDGET_HPP

#include <QWidget>
#include <QtGui/QStandardItemModel>

namespace Elpida
{

	namespace Ui
	{
		class TaskResultsWidget;
	}

	class TaskResultsWidget : public QWidget
	{
	Q_OBJECT

	public:
		explicit TaskResultsWidget();
		~TaskResultsWidget();
	private:
		QStandardItemModel _model;
		Ui::TaskResultsWidget* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_TASKRESULTSWIDGET_HPP
