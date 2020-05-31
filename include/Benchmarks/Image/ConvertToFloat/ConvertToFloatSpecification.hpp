//
// Created by klapeto on 31/5/20.
//

#ifndef INCLUDE_BENCHMARKS_IMAGE_CONVERTTOFLOAT_CONVERTTOFLOATSPECIFICATION_HPP
#define INCLUDE_BENCHMARKS_IMAGE_CONVERTTOFLOAT_CONVERTTOFLOATSPECIFICATION_HPP

#include <Elpida/Engine/Task/TaskSpecification.hpp>

namespace Elpida{
	class ConvertToFloatSpecification final: public TaskSpecification
	{
	public:
		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun) const override;
		ConvertToFloatSpecification();
		~ConvertToFloatSpecification() override = default;
	};
}

#endif //INCLUDE_BENCHMARKS_IMAGE_CONVERTTOFLOAT_CONVERTTOFLOATSPECIFICATION_HPP
