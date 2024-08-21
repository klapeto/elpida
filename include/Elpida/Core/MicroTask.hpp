//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_MICROTASK_HPP_
#define ELPIDA_MICROTASK_HPP_

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

		MicroTask() = default;
		~MicroTask() override = default;
	protected:
		void DoRun() final;
		virtual void DoRun(Iterations iterations) = 0;
		virtual Size GetOperationsPerformedPerRun() = 0;
		virtual Duration GetExecutionMinimumDuration() = 0;
		virtual void OnBeforeRun(Iterations iterations);
	};

} // Elpida

#endif //ELPIDA_MICROTASK_HPP_
