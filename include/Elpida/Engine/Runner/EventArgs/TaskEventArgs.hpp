//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_TASKEVENTARGS_HPP
#define INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_TASKEVENTARGS_HPP


namespace Elpida
{
	class TaskSpecification;

	class TaskEventArgs
	{
	public:
		[[nodiscard]] const TaskSpecification& getSpecification() const
		{
			return _specification;
		}

		explicit TaskEventArgs(const TaskSpecification& specification)
			: _specification(specification)
		{

		}
		virtual ~TaskEventArgs() = default;
	private:
		const TaskSpecification& _specification;
	};

}


#endif //INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_TASKEVENTARGS_HPP
