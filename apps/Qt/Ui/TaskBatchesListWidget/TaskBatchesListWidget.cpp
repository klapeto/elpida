#include "TaskBatchesListWidget.hpp"
#include "ui_TaskBatchesListWidget.h"

#include <QList>

#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/TaskBatch.hpp>
#include <Elpida/Task.hpp>
#include <Elpida/Utilities/Plugin/Plugin.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Utilities/Plugin/TaskBatchesContainerPlugin.hpp>
#include <TaskBatches/QtTaskBatchWrapper.hpp>

namespace Elpida
{

	TaskBatchesListWidget::TaskBatchesListWidget(const SharedLibraryLoader& loader, Logger& logger)
		: QWidget(), _ui(new Ui::TaskBatchesListWidget), _loader(loader), _logger(logger)
	{
		_ui->setupUi(this);
		_ui->lvTaskBatches->setModel(&_model);
	}

	TaskBatchesListWidget::~TaskBatchesListWidget()
	{
		delete _ui;
		destroyAllBatches();
	}

	void TaskBatchesListWidget::reloadTaskBatches()
	{
		destroyAllBatches();
		const auto& loaded = _loader.getLoadedLibraries();
		for (const auto& lib: loaded)
		{
			auto factoryFp = lib.second
				.getFunctionPointer<TaskBatchesContainerPlugin<Elpida::QtTaskBatchWrapper>::Factory>("createPlugin");
			if (factoryFp != nullptr)
			{
				TaskBatchesContainerPlugin<Elpida::QtTaskBatchWrapper>* pPlugin = nullptr;
				try
				{
					pPlugin = factoryFp();
				}
				catch (const std::exception& ex)
				{
					_logger.log(LogType::Error,
						Vu::concatenateToString("Plugin with name: '",
							lib.first,
							"' failed to produce task batches"),
						ex);
					throw;
				}

				const auto& data = pPlugin->getUnderlyingData();
				_createdPlugins.push_back(pPlugin);
				for (auto bWrapper : data)
				{
					const auto& batch = bWrapper->getTaskBatch();
					auto item = new QStandardItem(QString::fromStdString(batch.getName()));
					item->setData(QVariant::fromValue(bWrapper));
					_model.appendRow(item);
				}
			}
		}
	}

	void TaskBatchesListWidget::destroyAllBatches()
	{
		for (auto plugin: _createdPlugins)
		{
			delete plugin;
		}
		_createdPlugins.clear();
		_model.clear();
	}

	QtTaskBatchWrapper* TaskBatchesListWidget::getSelectedTaskBatch()
	{
		auto selectedIndexes = _ui->lvTaskBatches->selectionModel()->selectedIndexes();
		if (!selectedIndexes.empty())
		{
			auto variant = _model.itemFromIndex(selectedIndexes.first())->data();
			// Dirty hack because QtTaskBatchWrapper is derived from QWidget and QVariant special handles that
			// which we do not want here.
			return (QtTaskBatchWrapper*)variant.value<void*>();
		}
		return nullptr;
	}

} // namespace Elpida
