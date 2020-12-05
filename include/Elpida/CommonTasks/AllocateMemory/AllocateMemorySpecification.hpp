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

//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_COMMONTASKS_ALLOCATEMEMORY_ALLOCATEMEMORYSPECIFICATION_HPP
#define INCLUDE_ELPIDA_COMMONTASKS_ALLOCATEMEMORY_ALLOCATEMEMORYSPECIFICATION_HPP

#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{
	class AllocateMemorySpecification final : public TaskSpecification
	{
	public:
		class Settings
		{
		public:
			static inline const char* MemorySize = "Memory size";
		};

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun,
			size_t iterationsToRun) const override;

		AllocateMemorySpecification();
		~AllocateMemorySpecification() override = default;
	};
}


#endif //INCLUDE_ELPIDA_COMMONTASKS_ALLOCATEMEMORY_ALLOCATEMEMORYSPECIFICATION_HPP
