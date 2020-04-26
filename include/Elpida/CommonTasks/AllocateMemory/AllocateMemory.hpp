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
 * AllocateMemory.hpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_COMMONTASKS_ALLOCATEMEMORY_HPP_
#define ELPIDA_COMMONTASKS_ALLOCATEMEMORY_HPP_

#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Utilities/NumaMemory.hpp"


namespace Elpida
{
	class Memory;

	class AllocateMemory final : public Task
	{
	public:
		void execute() override;

		AllocateMemory(const TaskSpecification& specification,
			const TaskAffinity& affinity,
			bool toBeCountedOnResults,
			std::size_t size,
			bool initialize = false);

		~AllocateMemory() override = default;
	protected:
		void prepareImpl() override;
		TaskData finalizeAndGetOutputData() override;
	private:
		NumaMemory _memory;
		std::size_t _size;
		bool _initialize;
	};

} /* namespace Elpida */

#endif /* ELPIDA_COMMONTASKS_ALLOCATEMEMORY_HPP_ */
