//
// Created by klapeto on 31/5/20.
//

#ifndef SRC_BENCHMARKS_IMAGE_PNGDECODING_PNGDECODINGSPECIFICATION_HPP
#define SRC_BENCHMARKS_IMAGE_PNGDECODING_PNGDECODINGSPECIFICATION_HPP


#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{
	class PngDecodingSpecification : public TaskSpecification
	{
	public:
		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun) const override;

		PngDecodingSpecification();
		~PngDecodingSpecification() override = default;
	};
}

#endif //SRC_BENCHMARKS_IMAGE_PNGDECODING_PNGDECODINGSPECIFICATION_HPP
