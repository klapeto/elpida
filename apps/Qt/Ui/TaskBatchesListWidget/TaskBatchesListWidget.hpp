#ifndef APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
#define APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP

#include <QWidget>
#include <QtGui/QStandardItemModel>

namespace Elpida
{
	template <typename T>
	class TaskBatchesContainerPlugin;

	class QtTaskBatchWrapper;
	class SharedLibraryLoader;
	class Logger;

	namespace Ui
	{
		class TaskBatchesListWidget;
	}

	class TaskBatchesListWidget : public QWidget
	{
		Q_OBJECT

	public:

		void reloadTaskBatches();
		void destroyAllBatches();

		QtTaskBatchWrapper* getSelectedTaskBatch();

		explicit TaskBatchesListWidget(const SharedLibraryLoader& loader, Logger& logger);
		~TaskBatchesListWidget() override;
	private:
		Ui::TaskBatchesListWidget* _ui;
		const SharedLibraryLoader& _loader;
		Logger& _logger;
		std::vector<TaskBatchesContainerPlugin<QtTaskBatchWrapper>*> _createdPlugins;
		QStandardItemModel _model;
	};

} // namespace Elpida
#endif // APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
