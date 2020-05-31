//
// Created by klapeto on 31/5/20.
//

#ifndef INCLUDE_BENCHMARKS_IMAGE_IMAGETASKBASE_HPP
#define INCLUDE_BENCHMARKS_IMAGE_IMAGETASKBASE_HPP


#include <Elpida/Engine/Task/Task.hpp>

namespace Elpida
{
	class ImageTaskBase : public Task
	{
	public:
		ImageTaskBase(const TaskSpecification& specification, const ProcessorNode& processorToRun);
		~ImageTaskBase() override = default;
	protected:
		struct InputImageProperties
		{
			size_t width;
			size_t height;
		};

		static InputImageProperties getImageProperties(const TaskDataDto& taskData);
	};
}

#endif //INCLUDE_BENCHMARKS_IMAGE_IMAGETASKBASE_HPP
