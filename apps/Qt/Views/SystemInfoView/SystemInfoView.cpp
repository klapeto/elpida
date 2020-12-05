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
#include <Elpida/Utilities/ValueUtilities.hpp>

#include <cmath>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

namespace Elpida
{

	SystemInfoView::SystemInfoView(const CpuInfo& cpuInfo, const SystemTopology& topology)
		: QWidget(), _ui(new Ui::SystemInfoView)
	{
		_ui->setupUi(this);

		loadCpuInfo(cpuInfo, topology);
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

		for (auto& cache: caches)
		{
			auto group = new QGroupBox(QString::fromStdString(cache.getName()));

			auto* groupLayout = new QGridLayout();

			groupLayout->addWidget(new QLabel(QString::fromStdString("Size")), 0, 0);
			groupLayout
				->addWidget(new QLabel(QString::fromStdString(Vu::getValueScaleStringSI(cache.getSize()) + "B")), 0, 1);

			groupLayout->addWidget(new QLabel(QString::fromStdString("Associativity")), 1, 0);
			groupLayout->addWidget(new QLabel(QString::fromStdString(cache.getAssociativity())), 1, 1);

			groupLayout->addWidget(new QLabel(QString::fromStdString("Lines Per Tag")), 2, 0);
			groupLayout->addWidget(new QLabel(QString::fromStdString(std::to_string(cache.getLinesPerTag()))), 2, 1);

			groupLayout->addWidget(new QLabel(QString::fromStdString("Lines size")), 3, 0);
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

		_ui->gbCaches->setLayout(cacheLayout);
	}

	void SystemInfoView::loadFeatures(const CpuInfo& cpuInfo)
	{
		auto featuresLayout = new QGridLayout();

		auto features = cpuInfo.getFeatures();

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

		_ui->gbCpuFreatures->setLayout(featuresLayout);
		_ui->gbCpuFreatures->setAttribute(Qt::WA_TransparentForMouseEvents);
	}

	void SystemInfoView::loadBasicInfo(const CpuInfo& cpuInfo, const SystemTopology& topology)
	{
		auto& additionalInfoMap = cpuInfo.getAdditionalInformation();

		auto infoLayout = new QGridLayout();

		std::vector<std::pair<std::string, std::string>> info;

		info.emplace_back("Vendor", cpuInfo.getVendorString());
		info.emplace_back("Brand", cpuInfo.getProcessorBrand());
		info.emplace_back("Physical Cores", std::to_string(topology.getTotalPhysicalCores()));
		info.emplace_back("Logical Cores", std::to_string(topology.getTotalLogicalCores()));

		info.emplace_back("Base Frequency", Vu::getValueScaleStringSI(cpuInfo.getFrequency()) + "HZ");

		for (auto& infoPair: additionalInfoMap)
		{
			info.emplace_back(infoPair);
		}

		auto sideSize = ceil(std::sqrt(info.size()));
		auto row = 0;
		auto column = 0;

		for (const auto& infoPair: info)
		{
			infoLayout->addWidget(new QLabel(QString::fromStdString(infoPair.first)), row, column);
			infoLayout->addWidget(new QLabel(QString::fromStdString(infoPair.second)), row, ++column);

			if (++column >= sideSize)
			{
				row++;
				column = 0;
			}
		}

		_ui->gbCpuInfo->setLayout(infoLayout);
	}

} // namespace Elpida
