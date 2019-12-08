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

/*
 * MemoryReadProperties.cpp
 *
 *  Created on: 1 Δεκ 2019
 *      Author: klapeto
 */

#include "TaskBatches/Memory/Ui/MemoryReadProperties.hpp"
#include "Elpida/Exceptions/ElpidaException.hpp"
#include "ui_MemoryReadProperties.h"

namespace Elpida
{
	MemoryReadProperties::MemoryReadProperties(MultithreadMemoryChunksReadTaskBatch* taskBatch)
			:
			  MemoryBandwidthChart<MultithreadMemoryChunksReadTaskBatch>(taskBatch, true),
			  _ui(new Ui::MemoryReadProperties),
			  _sizePerThread(256),
			  _autoConfigureSizes(true)
	{
		_ui->setupUi(this);
		_connections.push_back(
		        _ui->chkAutomaticSize->connect(_ui->chkAutomaticSize, &QCheckBox::stateChanged, this,
		                                       &MemoryReadProperties::chkAutomaticSize_stateChanged));
		_ui->chkAutomaticSize->setChecked(true);
		_ui->sbMemorySize->setEnabled(false);
		_ui->lblMemorySizePerThread->setEnabled(false);
	}

	void MemoryReadProperties::reconfigureTaskBatch()
	{
		_taskBatch->setSizePerThread(_ui->sbMemorySize->value());
		_taskBatch->setAutoConfigureSizes(_ui->chkAutomaticSize->isChecked());
	}

	void MemoryReadProperties::validateConfiguration()
	{
		if (!_ui->chkAutomaticSize->isChecked())
		{
			auto size = _ui->sbMemorySize->value();
			while (size % (32 * sizeof(int64_t)) != 0)
			{
				size--;
			}
			_ui->sbMemorySize->setValue(size);
		}
	}

	MemoryReadProperties::~MemoryReadProperties()
	{
		for (auto& connection : _connections)
		{
			QObject::disconnect(connection);
		}
		delete _ui;
	}

	void MemoryReadProperties::chkAutomaticSize_stateChanged(int state)
	{
		_autoConfigureSizes = (bool) state;
		_ui->sbMemorySize->setEnabled(!_autoConfigureSizes);
		_ui->lblMemorySizePerThread->setEnabled(!_autoConfigureSizes);
	}

} /* namespace Elpida */

