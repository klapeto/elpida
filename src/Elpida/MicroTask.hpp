//
// Created by klapeto on 27/2/2023.
//

#ifndef _MICROTASK_HPP_
#define _MICROTASK_HPP_

#include <cstdlib>

#include "Elpida/Task.hpp"

namespace Elpida
{

	class MicroTask : public Task
	{
	 public:
		Duration Run() final;

		MicroTask() = default;
		~MicroTask() override = default;
	 protected:
		void DoRun() final;
		virtual void DoRun(std::size_t iterations) = 0;
		virtual std::size_t GetOperationsPerformedPerRun() = 0;
		virtual Duration GetExecutionMinimumDuration() = 0;
	};

} // Elpida

#endif //_MICROTASK_HPP_
