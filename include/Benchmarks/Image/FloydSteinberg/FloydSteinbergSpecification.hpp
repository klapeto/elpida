//
// Created by klapeto on 31/5/20.
//

#ifndef INCLUDE_BENCHMARKS_IMAGE_FLOYDSTEINBERG_FLOYDSTEINBERGSPECIFICATION_HPP
#define INCLUDE_BENCHMARKS_IMAGE_FLOYDSTEINBERG_FLOYDSTEINBERGSPECIFICATION_HPP

#include <Elpida/Engine/Task/TaskSpecification.hpp>
namespace Elpida
{
	class FloydSteinbergSpecification : public TaskSpecification
	{
	public:
		class Settings
		{
		public:
			static inline const char* Threshold = "Threshold";
		};

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun) const override;

		FloydSteinbergSpecification();
		~FloydSteinbergSpecification() override = default;
	};
}

#endif //INCLUDE_BENCHMARKS_IMAGE_FLOYDSTEINBERG_FLOYDSTEINBERGSPECIFICATION_HPP
