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
#include <thread>

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

		private:
			Runner _taskBatchRunner;
			std::thread _runnerThread;
			const Map<String, QtTaskBatchWrapper*>& _taskBatchList;
			Ui::RunTaskBatchDialog *_ui;

			void onTaskBatchListModified();

	};

} // namespace Elpida
#endif // ELPIDA_RUNTASKBATCHDIALOG_HPP
