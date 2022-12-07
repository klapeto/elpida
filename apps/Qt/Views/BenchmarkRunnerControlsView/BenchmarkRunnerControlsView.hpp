/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#ifndef ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP
#define ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP

#include <QWidget>

namespace Elpida
{

	namespace Ui
	{
		class BenchmarkRunnerControlsView;
	}

	class BenchmarkRunnerModel;
	class EventSubscriptionBase;
	class GlobalConfigurationModel;
	class Mediator;

	class BenchmarkRunnerControlsView : public QWidget
	{
	Q_OBJECT

	public:
		explicit BenchmarkRunnerControlsView(Mediator& mediator, const BenchmarkRunnerModel& model, GlobalConfigurationModel& globalConfigurationModel);
		~BenchmarkRunnerControlsView() override;
	private:
		Ui::BenchmarkRunnerControlsView* _ui;
		const BenchmarkRunnerModel& _model;
		EventSubscriptionBase* _dataChangedEventSubscription;
		EventSubscriptionBase* _configurationChangedEventSubscription;
		GlobalConfigurationModel& _globalConfigurationModel;
		Mediator& _mediator;
	signals:
		void onDataChanged();
		void onConfigurationChanged();
	private slots:
		void updateUi();
		void startClicked(bool checked);
		void stopClicked(bool checked);
		void onUploadStateChanged(int state);
		void onOpenWebPageStateChanged(int state);
		void onBlockUIStateChanged(int state);
		void updateCheckboxes() const;
	};

} // namespace Elpida

#endif // ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP
