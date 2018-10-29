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

#ifndef ELPIDA_RUNTASKBATCHDIALOG_HPP
#define ELPIDA_RUNTASKBATCHDIALOG_HPP

#include <QDialog>
#include <Elpida/Types/String.hpp>
#include <Elpida/Runner.hpp>
#include <Elpida/Types/Map.hpp>
#include <Elpida/OffThreadExecutor.hpp>

namespace Elpida
{

	class QtTaskBatchWrapper;

	namespace Ui
	{
		class RunTaskBatchDialog;
	}

	class RunTaskBatchDialog final: public QDialog
	{
		Q_OBJECT

		public:
			explicit RunTaskBatchDialog(const Map<String, QtTaskBatchWrapper*>& taskBatchList, QWidget *parent = nullptr);
			~RunTaskBatchDialog();

		private slots:
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
			const Map<String, QtTaskBatchWrapper*>& _taskBatchList;
			QString _runningText;
			QString _readyText;
			QString _na;
			Ui::RunTaskBatchDialog *_ui;

			void onTaskBatchListModified();
			void appendResults();
			void runTaskBatches();

			signals:
			void onSessionBegin();
			void onTaskBatchStart(const QString& name, int size);
			void onTaskStart(const QString& name);
			void onTaskBatchEnd(const QString& name);
			void onSessionEnd();
	};

} // namespace Elpida
#endif // ELPIDA_RUNTASKBATCHDIALOG_HPP
