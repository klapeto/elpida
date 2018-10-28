/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#include "ui_MainWindow.h"
#include <Elpida/CpuInfo.hpp>
#include <Elpida/Task.hpp>
#include <TaskBatches/Config.hpp>
#include <Elpida/TaskBatch.hpp>

#include <QMessageBox>
#include <QTreeWidgetItem>
#include "Tools/Qt/MainWindow.hpp"

#include "TaskBatches/QtTaskBatchWrapper.hpp"
#include "TaskBatchProperties.hpp"
#include "RunTaskBatchDialog.hpp"
#include "LogsDialog.hpp"

#define PAGE_CREATION_FUNCTION_NAME "createQtBatchWrapper"

namespace Elpida
{

	MainWindow::MainWindow(QWidget *parent)
			: QMainWindow(parent), _ui(new Ui::MainWindow), _fixedSizeSet(false)
	{
		_ui->setupUi(this);

		_taskBatchPropertiesDialog = new TaskBatchProperties(_ui->centralWidget);

		loadTaskInfo();
		_runTaskBatchDialog = new RunTaskBatchDialog(_createdPropetyPages, _ui->centralWidget);

		_logsDialog = new LogsDialog(_ui->centralWidget);

		loadCpuInfo();
		//addMascot();

		_connections.push_back(
		        QTreeWidget::connect(_ui->twTasks, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onTwTaskBatchListDoubleClick));
	}

	void MainWindow::addMascot()
	{
	}

	void MainWindow::showEvent(QShowEvent *event)
	{
		QMainWindow::showEvent(event);
		setFixedSize(sizeHint());
	}

	MainWindow::~MainWindow()
	{
		delete _ui;
		for (auto& pair : _createdPropetyPages)
		{
			if (pair.second != nullptr)
			{
				delete pair.second;
			}
		}
	}

	void MainWindow::onTwTaskBatchListDoubleClick(QTreeWidgetItem* item, int col)
	{
		auto taskBatchName = item->text(0).toStdString();
		auto pageItr = _createdPropetyPages.find(taskBatchName);
		if (pageItr != _createdPropetyPages.end())
		{
			if (pageItr->second != nullptr)
			{
				_taskBatchPropertiesDialog->setPage(pageItr->second);
				_taskBatchPropertiesDialog->show();
			}
			else
			{
				QMessageBox::information(_ui->centralWidget, "Task Batch Properties", "This task batch does not export properties",
				                         QMessageBox::StandardButton::Ok);
			}
		}
	}

	void MainWindow::loadCpuInfo(void)
	{
		auto cpuInfo = Elpida::CpuInfo::getCpuInfo();
		_ui->lblVendorValue->setText(cpuInfo.getVendorString().c_str());
		_ui->lblModelValue->setText(cpuInfo.getProcessorBrand().c_str());
		_ui->lblFamilyValue->setText(QString::number(cpuInfo.getFamily()));
		_ui->lblSteppingValue->setText(QString::number(cpuInfo.getStepping()));
		_ui->lblTscFreqValue->setText(QString::number(cpuInfo.getTscFequency() / std::giga::num, 'g', 3) + QString(" GHZ"));
		_ui->lblLogicalCoresValue->setText(QString::number(cpuInfo.getLogicalProcessors()));
		_ui->chkHyperthreading->setChecked(cpuInfo.isHyperThreading());
		_ui->chkTurbo->setChecked(cpuInfo.isTurboBoost());
		_ui->chkTurbo3->setChecked(cpuInfo.isTurboBoost3());

		auto instructions = cpuInfo.getInstructionSetSupport();
		_ui->gbCpuFreatures->setAttribute(Qt::WA_TransparentForMouseEvents);
		_ui->chkCmov->setChecked(instructions["CMOV"].isSupported());
		_ui->chkMmx->setChecked(instructions["MMX"].isSupported());
		_ui->chkMmxExt->setChecked(instructions["MmxExt"].isSupported());
		_ui->chkSse->setChecked(instructions["SSE"].isSupported());
		_ui->chkSse2->setChecked(instructions["SSE2"].isSupported());
		_ui->chkSse3->setChecked(instructions["SSE3"].isSupported());
		_ui->chkSsse3->setChecked(instructions["SSSE3"].isSupported());
		_ui->chkSse41->setChecked(instructions["SSE41"].isSupported());
		_ui->chkSse42->setChecked(instructions["SSE42"].isSupported());
		_ui->chkSse4a->setChecked(instructions["SSE4A"].isSupported());
		_ui->chkFma->setChecked(instructions["FMA"].isSupported());
		_ui->chkFma4->setChecked(instructions["FMA4"].isSupported());
		_ui->chkAes->setChecked(instructions["AES"].isSupported());
		_ui->chkAvx->setChecked(instructions["AVX"].isSupported());
		_ui->chkAvx2->setChecked(instructions["AVX2"].isSupported());
		_ui->chkAvx512->setChecked(instructions["AVX512-F"].isSupported());
		_ui->chkAvx512bw->setChecked(instructions["AVX512-BW"].isSupported());
		_ui->chkAvx512dq->setChecked(instructions["AVX512-DQ"].isSupported());
		_ui->chkAvx512Fma->setChecked(instructions["AVX512-IFMA"].isSupported());
		_ui->chkAvx512pf->setChecked(instructions["AVX512-PF"].isSupported());
		_ui->chkAvx512er->setChecked(instructions["AVX512-ER"].isSupported());
		_ui->chkAvx512cd->setChecked(instructions["AVX512-CD"].isSupported());
		_ui->chkAvx512vbmi->setChecked(instructions["AVX512-VBMI"].isSupported());
		_ui->chkSha->setChecked(instructions["SHA"].isSupported());
		_ui->chkXop->setChecked(instructions["XOP"].isSupported());
		_ui->chk3DNow->setChecked(instructions["3DNow"].isSupported());
		_ui->chk3DNowext->setChecked(instructions["3DNowExt"].isSupported());
		_ui->chk3DNowPre->setChecked(instructions["3DNowPrefetch"].isSupported());
		_ui->chkBmi1->setChecked(instructions["BMI1"].isSupported());
		_ui->chkBmi2->setChecked(instructions["BMI2"].isSupported());
		_ui->chkAbm->setChecked(instructions["ABM"].isSupported());
		_ui->chkF16c->setChecked(instructions["F16C"].isSupported());
		_ui->chkRdrand->setChecked(instructions["RDRAND"].isSupported());
		_ui->chkAvx512Vbmi2->setChecked(instructions["AVX512-VBMI2"].isSupported());
		_ui->chkAvx512Vl->setChecked(instructions["AVX512-VL"].isSupported());
		_ui->chkAvx512Vnni->setChecked(instructions["AVX512-VNNI"].isSupported());
		_ui->chkAvx5124vnniw->setChecked(instructions["AVX512-4VNNIW"].isSupported());
		_ui->chkAvx512Bitalg->setChecked(instructions["AVX512-BITALG"].isSupported());
		_ui->chkAvx512Vp1->setChecked(instructions["AVX512-VPOPCNTDQ"].isSupported());
		_ui->chkAvx512Vp2->setChecked(instructions["AVX512-VPCLMULQDQ"].isSupported());
		_ui->chkAvx5124fmaps->setChecked(instructions["AVX512-4FMAPS"].isSupported());
		_ui->chkAvx512Gfni->setChecked(instructions["AVX512-GFNI"].isSupported());
		_ui->chkAvx512Vaes->setChecked(instructions["AVX512-VAES"].isSupported());
		_ui->chkSgx->setChecked(instructions["SGX"].isSupported());

		const auto& caches = cpuInfo.getCaches();

		if (caches.size() > 0)
		{
			const auto& cache = caches[0];	// L1 Inst
			_ui->lblL1InstSizeValue->setText(QString::number(cache.size / 1000) + " KB");
			_ui->lblL1InstAssocValue->setText(cache.associativity.c_str());
			_ui->lblL1InstLptValue->setText(QString::number(cache.linesPerTag));
			_ui->lblL1InstLsValue->setText(QString::number(cache.lineSize));
		}

		if (caches.size() > 1)
		{
			const auto& cache = caches[1];	// L1 Data
			_ui->lblL1DataSizeValue->setText(QString::number(cache.size / 1000) + " KB");
			_ui->lblL1DataAssocValue->setText(cache.associativity.c_str());
			_ui->lblL1DataLptValue->setText(QString::number(cache.linesPerTag));
			_ui->lblL1DataLsValue->setText(QString::number(cache.lineSize));
		}

		if (caches.size() > 2)
		{
			const auto& cache = caches[2];	// L2
			_ui->lblL2SizeValue->setText(QString::number(cache.size / 1000) + " KB");
			_ui->lblL2AssocValue->setText(cache.associativity.c_str());
			_ui->lblL2LptValue->setText(QString::number(cache.linesPerTag));
			_ui->lblL2LsValue->setText(QString::number(cache.lineSize));
		}

		if (caches.size() > 3)
		{
			const auto& cache = caches[3];	// L3
			_ui->lblL3SizeValue->setText(QString::number(cache.size / 1000) + " KB");
			_ui->lblL3AssocValue->setText(cache.associativity.c_str());
			_ui->lblL3LptValue->setText(QString::number(cache.linesPerTag));
			_ui->lblL3LsValue->setText(QString::number(cache.lineSize));
		}

	}

	void MainWindow::on_actionExit_triggered()
	{
		QApplication::quit();
	}

	void MainWindow::on_actionAbout_triggered()
	{
		QMessageBox::about(
		        _ui->centralWidget,
		        "About: Elpida",
		        "Elpida is an open source x86 Cpu/Algorithm benchmarking tool. It is released under the General Public License v3 (GPL v3). More info at: https://github.com/klapeto/elpida");
	}

	void MainWindow::loadTaskInfo()
	{
		try
		{
			_elpidaManager.setPluginDirectory("./Elpida");
			_elpidaManager.reloadPlugins();
			auto& plugins = _elpidaManager.getPluginLoader().getLoadedPlugins();
			for (auto& plugin : plugins)
			{
				auto func = plugin.second.getFunctionPointer<Elpida::QtTaskBatchWrapper* (*)()>(PAGE_CREATION_FUNCTION_NAME);
				if (func != nullptr)
				{
					auto baseItem = new QTreeWidgetItem(_ui->twTasks);
					auto prop = func();
					auto& taskBatch = prop->getTaskBatch();
					_createdPropetyPages.emplace(taskBatch.getName(), prop);
					baseItem->setText(0, QString(taskBatch.getName().c_str()));
					baseItem->setText(1, QString("True"));
					auto& tasks = taskBatch.getTasks();
					size_t c = 0;
					for (auto task : tasks)
					{
						auto taskItem = new QTreeWidgetItem(baseItem);
						taskItem->setText(0, QString::number(c++));
						taskItem->setText(1, QString(task->getName().c_str()));
					}
				}

			}
		}
		catch (ElpidaException& e)
		{
			QMessageBox::critical(_ui->centralWidget, "Error", QString::fromStdString(e.getMessage()), QMessageBox::StandardButton::Ok);
		}

	}

	void MainWindow::on_actionRunBatches_triggered()
	{
		_runTaskBatchDialog->show();
	}

	void Elpida::MainWindow::on_actionShowLogs_triggered()
	{
		_logsDialog->setLogsText(_elpidaManager.getLog());
		_logsDialog->show();
	}

}  // namespace Elpida

