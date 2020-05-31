//
// Created by klapeto on 31/5/20.
//

#ifndef SRC_BENCHMARKS_IMAGE_PNGENCODING_PNGENCODINGSPECIFICATION_HPP
#define SRC_BENCHMARKS_IMAGE_PNGENCODING_PNGENCODINGSPECIFICATION_HPP

#include <Elpida/Engine/Task/TaskSpecification.hpp>

namespace Elpida
{
	class PngEncodingSpecification : public TaskSpecification
	{
	public:

		virtual Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun) const override;

		PngEncodingSpecification();
		~PngEncodingSpecification() override = default;
	};
}


#endif //SRC_BENCHMARKS_IMAGE_PNGENCODING_PNGENCODINGSPECIFICATION_HPP
