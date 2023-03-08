//
// Created by klapeto on 27/2/2023.
//

#ifndef _MICROTASK_HPP_
#define _MICROTASK_HPP_

#include "Elpida/Task.hpp"
#include "Elpida/Size.hpp"

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
		virtual void DoRun(Size iterations) = 0;
		virtual Size GetOperationsPerformedPerRun() = 0;
		virtual Duration GetExecutionMinimumDuration() = 0;
	};

} // Elpida

#endif //_MICROTASK_HPP_
