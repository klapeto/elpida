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

#include "TaskBatches/Config.hpp"
#include <Elpida/Types/Array.hpp>
#include "TaskBatches/Memory/MemoryReadCachedTaskBatch.hpp"
#include "TaskBatches/Memory/MemoryReadVolatileTaskBatch.hpp"
#include "TaskBatches/Memory/MultithreadMemoryChunksReadCachedTaskBatch.hpp"

#if _elpida_qt_enabled
#include "TaskBatches/Memory/MemoryTasksPropertiesWithChart.hpp"
#include "TaskBatches/Memory/MemoryTasksProperties.hpp"
#endif

extern "C" Elpida::TaskBatch* createTaskBatch()
{
	return new Elpida::MemoryReadCachedTaskBatch();
}

#if _elpida_qt_enabled

extern "C" Elpida::QtTaskBatchWrapper* createQtBatchWrapper()
{
	return new Elpida::MemoryTasksPropertiesWithChart(new Elpida::MemoryReadCachedTaskBatch);
}

extern "C" Elpida::Array<Elpida::QtTaskBatchWrapper*>* createQtBatchWrappers()
{
	return new Elpida::Array<Elpida::QtTaskBatchWrapper*> {
		new Elpida::MemoryTasksPropertiesWithChart(new Elpida::MemoryReadCachedTaskBatch),
		new Elpida::MemoryTasksPropertiesWithChart(new Elpida::MemoryReadVolatileTaskBatch),
		new Elpida::MemoryTasksProperties<Elpida::MultithreadMemoryChunksReadCachedTaskBatch>(Elpida::MultithreadMemoryChunksReadCachedTaskBatch())
	};
}

#endif
