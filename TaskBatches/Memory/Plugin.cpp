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
 * Plugin.cpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include <vector>

#include "TaskBatches/Config.hpp"
#include "TaskBatches/Memory/Latency/MemoryLatency.hpp"
#include "TaskBatches/Memory/Latency/MemoryLatencyTaskBatch.hpp"
#include "TaskBatches/Memory/Read/MultithreadMemoryChunksReadTaskBatch.hpp"

#if _elpida_qt_enabled
#include "TaskBatches/Memory/Ui/MemoryBandwidthChart.hpp"
#include "TaskBatches/Memory/Ui/MemoryTasksProperties.hpp"
#include "TaskBatches/Memory/Ui/MemoryLatencyChart.hpp"
#endif

#if _elpida_qt_enabled

extern "C" std::vector<Elpida::QtTaskBatchWrapper*>* createQtBatchWrappers()
{
	return new std::vector<Elpida::QtTaskBatchWrapper*> {
		new Elpida::MemoryTasksProperties<Elpida::MultithreadMemoryChunksReadTaskBatch>(Elpida::MultithreadMemoryChunksReadTaskBatch()),
		new Elpida::MemoryLatencyChart(new Elpida::MemoryLatencyTaskBatch)
	};
}

#endif
