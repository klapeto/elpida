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

#ifndef ELPIDA_CPUINFOVIEW_HPP
#define ELPIDA_CPUINFOVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class CpuInfoView;
	}

	class CpuInfoModel;

	class CpuInfoView : public QWidget
	{
	Q_OBJECT

	public:
		explicit CpuInfoView(const CpuInfoModel& cpuInfo, QWidget* parent = nullptr);
		~CpuInfoView() override;
	private:
		Ui::CpuInfoView* _ui;
	};
}

#endif // ELPIDA_CPUINFOVIEW_HPP
