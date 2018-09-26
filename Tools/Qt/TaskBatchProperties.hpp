#ifndef TASKBATCHPROPERTIES_HPP
#define TASKBATCHPROPERTIES_HPP

#include <QDialog>

namespace Ui {
	class TaskBatchProperties;
}

class TaskBatchProperties : public QDialog
{
		Q_OBJECT

	public:
		void setPage(QWidget* widget);

		explicit TaskBatchProperties(QWidget *parent = 0);
		~TaskBatchProperties();

	private:
		Ui::TaskBatchProperties *ui;
};

#endif // TASKBATCHPROPERTIES_HPP
