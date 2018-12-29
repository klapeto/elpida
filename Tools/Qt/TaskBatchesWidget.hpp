#ifndef TASKBATCHESWIDGET_HPP
#define TASKBATCHESWIDGET_HPP

#include <QWidget>
#include <Elpida/Types/String.hpp>
#include <Elpida/Runner.hpp>
#include <Elpida/Types/Map.hpp>
#include <Elpida/OffThreadExecutor.hpp>

class QListWidgetItem;

namespace Elpida
{
	class QtTaskBatchWrapper;
	class TaskBatchProperties;

	namespace Ui
	{
		class TaskBatchesWidget;
	}

	class TaskBatchesWidget final: public QWidget
	{
		Q_OBJECT
		public:
			explicit TaskBatchesWidget(const Map<String, QtTaskBatchWrapper*>& taskBatchList, QWidget *parent = nullptr);
			~TaskBatchesWidget();

		public slots:
			void on_pbRun_clicked();
			void on_pbStop_clicked();

			void updateForSessionBegin();
			void updateForTaskBatchBegin(const QString& name, int size);
			void updateForTaskBegin(const QString& name);
			void updateForTaskBatchEnd(const QString& name);
			void updateForSessionEnd();

		private:
			Runner _taskBatchRunner;
			OffThreadExecutor _taskRunnerThread;

			QString _runningText;
			QString _readyText;
			QString _na;
			const Map<String, QtTaskBatchWrapper*>& _taskBatchList;
			Ui::TaskBatchesWidget *_ui;
			TaskBatchProperties* _taskBatchPropertiesDialog;

			void onTaskBatchListModified();
			void appendResults();
			void runTaskBatches();

		signals:
			void onSessionBegin();
			void onTaskBatchStart(const QString& name, int size);
			void onTaskStart(const QString& name);
			void onTaskBatchEnd(const QString& name);
			void onSessionEnd();
		private slots:
			void on_lwTaskBatches_itemDoubleClicked(QListWidgetItem *item);
	};

} // namespace Elpida

#endif // TASKBATCHESWIDGET_HPP
