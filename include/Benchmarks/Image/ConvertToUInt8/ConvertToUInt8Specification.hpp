//
// Created by klapeto on 31/5/20.
//

#ifndef INCLUDE_BENCHMARKS_IMAGE_CONVERTTOUINT8_CONVERTTOUINT8SPECIFICATION_HPP
#define INCLUDE_BENCHMARKS_IMAGE_CONVERTTOUINT8_CONVERTTOUINT8SPECIFICATION_HPP


#include <Elpida/Engine/Task/TaskSpecification.hpp>
namespace Elpida
{
	class ConvertToUInt8Specification : public TaskSpecification
	{
	public:
		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun) const override;

		ConvertToUInt8Specification();
		~ConvertToUInt8Specification() override = default;
	};
}

#endif //INCLUDE_BENCHMARKS_IMAGE_CONVERTTOUINT8_CONVERTTOUINT8SPECIFICATION_HPP
