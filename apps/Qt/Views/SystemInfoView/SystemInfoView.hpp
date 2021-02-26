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

#ifndef APPS_QT_UI_SYSTEMINFOWIDGET_SYSTEMINFOWIDGET_HPP
#define APPS_QT_UI_SYSTEMINFOWIDGET_SYSTEMINFOWIDGET_HPP

#include <QWidget>

namespace Elpida
{

	class CpuInfo;
	class SystemTopology;
	class Mediator;
	class OsInfo;
	class TimingInfo;
	class MemoryInfo;

	namespace Ui
	{
		class SystemInfoView;
	}

	class SystemInfoView : public QWidget
	{
	Q_OBJECT

	public:
		explicit SystemInfoView(const CpuInfo& cpuInfo,
			const SystemTopology& topology,
			const OsInfo& osInfo,
			const TimingInfo& timingInfo,
			const MemoryInfo& memoryInfo);
		~SystemInfoView() override;

	private:
		Ui::SystemInfoView* _ui;

		void loadCpuInfo(const CpuInfo& cpuInfo, const SystemTopology& topology);
		void loadBasicInfo(const CpuInfo& cpuInfo, const SystemTopology& topology);
		void loadFeatures(const CpuInfo& cpuInfo);
		void loadCaches(const CpuInfo& cpuInfo);
		void loadOsInfo(const OsInfo& osInfo);
		void loadTimingInfo(const TimingInfo& timingInfo);
		void loadMemoryInfo(const MemoryInfo& memoryInfo);
	};

} // namespace Elpida

#endif // APPS_QT_UI_SYSTEMINFOWIDGET_SYSTEMINFOWIDGET_HPP
