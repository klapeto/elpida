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

//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_MICROTASK_HPP_
#define ELPIDA_MICROTASK_HPP_

#include <cmath>
#include <atomic>
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Task.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Iterations.hpp"

namespace Elpida
{

	class MicroTask : public Task
	{
	public:
		Duration Run() final;

		MicroTask();
		~MicroTask() override = default;
	protected:
		void DoRun() final;
		virtual void DoRunImpl() = 0;
		virtual Size GetOperationsPerformedPerRun() = 0;
		virtual Duration GetExecutionMinimumDuration();
	protected:
		std::size_t _iterations;
		std::atomic<bool> _keepGoing;

	protected:
		template<typename TCallable>
		inline void Exec(TCallable callable)
		{
			do
			{
				callable();
				_iterations++;
			} while (_keepGoing);
		}
	};

} // Elpida

#endif //ELPIDA_MICROTASK_HPP_
