//
// Created by klapeto on 31/5/20.
//

#ifndef INCLUDE_BENCHMARKS_IMAGE_GRAYSCALEAVERAGE_GRAYSCALEAVERAGESPECIFICATION_HPP
#define INCLUDE_BENCHMARKS_IMAGE_GRAYSCALEAVERAGE_GRAYSCALEAVERAGESPECIFICATION_HPP

#include <Elpida/Engine/Task/TaskSpecification.hpp>

namespace Elpida {
	class GrayscaleAverageSpecification: public TaskSpecification
	{
	public:
		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun) const override;

		GrayscaleAverageSpecification();
		~GrayscaleAverageSpecification() override = default;
	};
}

#endif //INCLUDE_BENCHMARKS_IMAGE_GRAYSCALEAVERAGE_GRAYSCALEAVERAGESPECIFICATION_HPP
