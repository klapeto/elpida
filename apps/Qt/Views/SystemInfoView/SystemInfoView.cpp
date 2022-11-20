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

#include "SystemInfoView.hpp"
#include "ui_SystemInfoView.h"

#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/SystemInfo/SystemTopology.hpp>
#include <Elpida/SystemInfo/OsInfo.hpp>
#include <Elpida/SystemInfo/TimingInfo.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>

#include <cmath>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

namespace Elpida
{

	SystemInfoView::SystemInfoView(const CpuInfo& cpuInfo,
			const SystemTopology& topology,
			const OsInfo& osInfo,
			const TimingInfo& timingInfo,
			const MemoryInfo& memoryInfo)
			: QWidget(), _ui(new Ui::SystemInfoView)
	{
		_ui->setupUi(this);

		loadCpuInfo(cpuInfo, topology);
		loadOsInfo(osInfo);
		loadTimingInfo(timingInfo);
		loadMemoryInfo(memoryInfo);
	}

	SystemInfoView::~SystemInfoView()
	{
		delete _ui;
	}

	void SystemInfoView::loadCpuInfo(const CpuInfo& cpuInfo, const SystemTopology& topology)
	{
		loadBasicInfo(cpuInfo, topology);

		loadFeatures(cpuInfo);

		loadCaches(cpuInfo);
	}

	void SystemInfoView::loadCaches(const CpuInfo& cpuInfo)
	{
		auto cacheLayout = new QVBoxLayout();

		auto& caches = cpuInfo.getCaches();

		std::vector<QLayout*> levelsLayouts;

		if (!caches.empty())
		{
			for (auto& cache: caches)
			{
				auto group = new QGroupBox(QString::fromStdString(cache.getName()));

				auto* groupLayout = new QGridLayout();

				groupLayout->addWidget(new QLabel(QString::fromStdString("Size: ")), 0, 0);
				groupLayout
						->addWidget(
								new QLabel(QString::fromStdString(Vu::getValueScaleStringSI(cache.getSize()) + "B")), 0,
								1);

				groupLayout->addWidget(new QLabel(QString::fromStdString("Associativity: ")), 1, 0);
				groupLayout->addWidget(new QLabel(QString::fromStdString(cache.getAssociativity())), 1, 1);

				groupLayout->addWidget(new QLabel(QString::fromStdString("Lines size: ")), 3, 0);
				groupLayout->addWidget(new QLabel(QString::fromStdString(std::to_string(cache.getLineSize()))), 3, 1);

				group->setLayout(groupLayout);

				while (levelsLayouts.size() < cache.getLevel())
				{
					levelsLayouts.push_back(new QHBoxLayout());
				}

				QLayout* layout = levelsLayouts[cache.getLevel() - 1];
				layout->addWidget(group);
			}
			for (auto layout: levelsLayouts)
			{
				cacheLayout->addLayout(layout, 0);
			}
		}
		else
		{
			_ui->gbCaches->setVisible(false);
		}

		_ui->gbCaches->setLayout(cacheLayout);
	}

	void SystemInfoView::loadFeatures(const CpuInfo& cpuInfo)
	{
		auto featuresLayout = new QGridLayout();

		auto features = cpuInfo.getFeatures();

		if (!features.empty())
		{
			std::sort(features.begin(), features.end(), [](const CpuFeature& a, const CpuFeature& b)
			{
				return a.getName() <= b.getName();
			});

			auto sideSize = floor(std::sqrt(features.size()));
			auto row = 0;
			auto column = 0;

			for (const auto& feature: features)
			{
				auto chkBox = new QCheckBox(QString::fromStdString(feature.getName()));
				chkBox->setChecked(feature.isSupported());
				featuresLayout->addWidget(chkBox, row, column);

				if (++column >= sideSize)
				{
					row++;
					column = 0;
				}
			}
		}
		else
		{
			_ui->gbCpuFreatures->setVisible(false);
		}

		_ui->gbCpuFreatures->setLayout(featuresLayout);
		_ui->gbCpuFreatures->setAttribute(Qt::WA_TransparentForMouseEvents);
	}

	void SystemInfoView::loadBasicInfo(const CpuInfo& cpuInfo, const SystemTopology& topology)
	{
		auto& additionalInfoMap = cpuInfo.getAdditionalInformation();

		std::vector<std::pair<std::string, std::string>> info;

		info.emplace_back("Architecture", cpuInfo.getArchitecture());
		info.emplace_back("Vendor", cpuInfo.getVendorName());
		info.emplace_back("Model name", cpuInfo.getModelName());
		info.emplace_back("Physical Cores", std::to_string(topology.getTotalPhysicalCores()));
		info.emplace_back("Logical Cores", std::to_string(topology.getTotalLogicalCores()));

		if (cpuInfo.getFrequency() > 0)
		{
			info.emplace_back("Base Frequency", Vu::getValueScaleStringSI(cpuInfo.getFrequency()) + "HZ");
		}

		for (auto& infoPair: additionalInfoMap)
		{
			info.emplace_back(infoPair);
		}

		auto sideSize = ceil(std::sqrt(info.size()));
		auto row = 0;
		auto column = 0;

		for (const auto& infoPair : info)
		{
			_ui->glInfo->addWidget(new QLabel(QString::fromStdString(infoPair.first) + ": "), row, column);
			_ui->glInfo->addWidget(new QLabel(QString::fromStdString(infoPair.second)), row, ++column);

			if (++column >= sideSize)
			{
				row++;
				column = 0;
			}
		}
	}

	void SystemInfoView::loadOsInfo(const OsInfo& osInfo)
	{
		auto osInfoLayout = new QGridLayout();

		osInfoLayout->addWidget(new QLabel(QString::fromStdString("Name: ")), 0, 0);
		osInfoLayout->addWidget(new QLabel(QString::fromStdString(osInfo.getName())), 0, 1);

		osInfoLayout->addWidget(new QLabel(QString::fromStdString("Version: ")), 1, 0);
		osInfoLayout->addWidget(new QLabel(QString::fromStdString(osInfo.getVersion())), 1, 1);

		_ui->gbOsInfo->setLayout(osInfoLayout);
	}

	static QWidget* getValueWidget(const std::string& value)
	{
		auto line = new QLabel(QString::fromStdString(value));

		line->setObjectName("valueLabel");
		return line;
	}

	static QWidget* getTargetTimeStringLabel(const TimingInfo& timingInfo)
	{
		using namespace std::chrono;
		auto seconds = duration_cast<duration<double, seconds::period>>(timingInfo.getTargetTime());

		if (!timingInfo.isTargetTimeFallback())
		{
			return getValueWidget(Vu::getValueScaleStringSI(seconds.count()) + "s");
		}
		else
		{
			auto widget = getValueWidget("<strong style=\"color: #d73e3e\">"
										 + Vu::getValueScaleStringSI(seconds.count()) + "s (!) </strong>");

			widget->setToolTip(
					"Fallback constant is used! Some running programs affect measurements. Please close all programs and restart Elpida.");

			return widget;
		}
	}

	static QWidget* createVerticalLine()
	{
		auto line = new QFrame();
		line->setFixedWidth(1);
		line->setLineWidth(1);
		line->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
		line->setStyleSheet(QString("background-color: rgb(179, 179, 179);"));
		return line;
	}

	void SystemInfoView::loadTimingInfo(const TimingInfo& timingInfo)
	{
		auto timingInfoLayout = new QGridLayout();

		auto column = 0;

		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("now() overhead: ")), 0, column);
		timingInfoLayout->addWidget(getValueWidget(
				Vu::getValueScaleStringSI((double)timingInfo.getNowOverhead().count() / Duration::period::den)
				+ "s"
		), 0, column + 1);

		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("sleep() overhead: ")), 1, column);
		timingInfoLayout->addWidget(getValueWidget(
				Vu::getValueScaleStringSI((double)timingInfo.getSleepOverhead().count() / Duration::period::den)
				+ "s"
		), 1, column + 1);

		column += 2;
		timingInfoLayout->addWidget(createVerticalLine(), 0, column++, 2, 1);

		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("lock() overhead: ")), 0, column);
		timingInfoLayout->addWidget(getValueWidget(
				Vu::getValueScaleStringSI((double)timingInfo.getLockOverhead().count() / Duration::period::den)
				+ "s"
		), 0, column + 1);

		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("wakeup() overhead: ")), 1, column);
		timingInfoLayout->addWidget(getValueWidget(
				Vu::getValueScaleStringSI((double)timingInfo.getWakeupOverhead().count() / Duration::period::den)
				+ "s"
		), 1, column + 1);

		column += 2;
		timingInfoLayout->addWidget(createVerticalLine(), 0, column++, 2, 1);

		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("notify() overhead: ")), 0, column);
		timingInfoLayout->addWidget(getValueWidget(
				Vu::getValueScaleStringSI((double)timingInfo.getNotifyOverhead().count() / Duration::period::den)
				+ "s"
		), 0, column + 1);


		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("join() overhead: ")), 1, column);
		timingInfoLayout->addWidget(getValueWidget(
				Vu::getValueScaleStringSI((double)timingInfo.getJoinOverhead().count() / Duration::period::den)
				+ "s"
		), 1, column + 1);


		column += 2;
		timingInfoLayout->addWidget(createVerticalLine(), 0, column++, 2, 1);


		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("Loop overhead: ")), 0, column);
		timingInfoLayout->addWidget(getValueWidget(
				Vu::getValueScaleStringSI((double)timingInfo.getLoopOverhead().count() / Duration::period::den)
				+ "s"
		), 0, column + 1);

		timingInfoLayout->addWidget(new QLabel(QString::fromStdString("Target time: ")), 1, column);
		timingInfoLayout->addWidget(getTargetTimeStringLabel(timingInfo), 1, column + 1);

		_ui->gbTimingInfo->setLayout(timingInfoLayout);
	}

	void SystemInfoView::loadMemoryInfo(const MemoryInfo& memoryInfo)
	{
		auto memoryInfoLayout = new QGridLayout();

		memoryInfoLayout->addWidget(new QLabel(QString::fromStdString("Total Size: ")), 0, 0);
		memoryInfoLayout->addWidget(new QLabel(
				QString::fromStdString(
						Vu::getValueScaleStringIEC(memoryInfo.getMemorySize()) + "B"
				)), 0, 1);

		memoryInfoLayout->addWidget(new QLabel(QString::fromStdString("Page Size: ")), 1, 0);
		memoryInfoLayout->addWidget(new QLabel(
				QString::fromStdString(
						Vu::getValueScaleStringIEC(memoryInfo.getPageSize()) + "B"
				)), 1, 1);

		_ui->gbMemoryInfo->setLayout(memoryInfoLayout);
	}

} // namespace Elpida
