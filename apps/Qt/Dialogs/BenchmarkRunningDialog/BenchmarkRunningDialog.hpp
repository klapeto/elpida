/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BENCHMARKRUNNINGDIALOG_HPP
#define BENCHMARKRUNNINGDIALOG_HPP

#include <QDialog>

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkRunningDialog;
	}

	class BenchmarkRunningDialog : public QDialog
	{
	Q_OBJECT

	public:
		explicit BenchmarkRunningDialog(QWidget* parent = nullptr);
		~BenchmarkRunningDialog() override;
	private:
		Ui::BenchmarkRunningDialog* _ui;

	private slots:
		void on_btnCancel_clicked(bool checked);
	};
}

#endif // BENCHMARKRUNNINGDIALOG_HPP
