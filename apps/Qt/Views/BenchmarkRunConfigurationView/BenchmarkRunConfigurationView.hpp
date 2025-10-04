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

#ifndef BENCHMARKRUNCONFIGURATIONVIEW_HPP
#define BENCHMARKRUNCONFIGURATIONVIEW_HPP

#include <QWidget>
#include "EventSubscription.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkRunConfigurationView;
	}

	class BenchmarkRunConfigurationController;
	class BenchmarkRunConfigurationModel;

	class BenchmarkRunConfigurationView : public QWidget
	{
	Q_OBJECT

	public:

		void DisableAdvancedSettings();
		void DisableUpload();
		void DisableIterations();
		void DisableDelay();
		void DisableHtmlReport();

		explicit BenchmarkRunConfigurationView(
				const BenchmarkRunConfigurationModel& model,
				BenchmarkRunConfigurationController& controller,
				QWidget* parent = nullptr);

		~BenchmarkRunConfigurationView() override;
	private:
		Ui::BenchmarkRunConfigurationView* _ui;
		const BenchmarkRunConfigurationModel& _model;
		BenchmarkRunConfigurationController& _controller;
		bool _uiUpdating;

		EventSubscription<> _modelChangedSubscription;
		void UpdateUi();
	private slots:

		void on_chkUpload_stateChanged(int state);

		void on_chkOpenResult_stateChanged(int state);

		void on_chkNumaAware_stateChanged(int state);

		void on_chkPinThreads_stateChanged(int state);

		void on_chkGenerateReport_stateChanged(int state);

		void on_spnTimes_valueChanged(int value);

		void on_spnMinimumMicroTaskTime_valueChanged(double value);

		void on_spnDelay_valueChanged(int value);

		void on_cmbConcurrencyMode_currentIndexChanged(int value);
	};
}
#endif // BENCHMARKRUNCONFIGURATIONVIEW_HPP
