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
 * MemoryReadProperties.hpp
 *
 *  Created on: 1 Δεκ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_UI_MEMORYREADPROPERTIES_HPP_
#define TASKBATCHES_MEMORY_UI_MEMORYREADPROPERTIES_HPP_

#include "TaskBatches/Memory/Ui/MemoryBandwidthChart.hpp"
#include "TaskBatches/Memory/Read/MultithreadMemoryChunksReadTaskBatch.hpp"

namespace Ui
{
	class MemoryReadProperties;
}

namespace Elpida
{

	class MemoryReadProperties final : public MemoryBandwidthChart<MultithreadMemoryChunksReadTaskBatch>
	{
	Q_OBJECT

	public:
		void reconfigureTaskBatch() override;
		void validateConfiguration() override;

		MemoryReadProperties(MultithreadMemoryChunksReadTaskBatch* taskBatch);
		~MemoryReadProperties();
	private:
		std::vector<QMetaObject::Connection> _connections;
		Ui::MemoryReadProperties* _ui;
		std::size_t _sizePerThread;
		bool _autoConfigureSizes;
	public slots:
		void chkAutomaticSize_stateChanged(int state);
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_UI_MEMORYREADPROPERTIES_HPP_ */
