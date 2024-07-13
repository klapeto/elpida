//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_TASKINFO_HPP
#define ELPIDA_TASKINFO_HPP

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ScoreComparisonType.hpp"
#include "Elpida/Core/ResultType.hpp"

namespace Elpida
{

	class TaskInfo final
	{
	 public:
		[[nodiscard]]
		const String& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const String& GetDescription() const
		{
			return _description;
		}

		[[nodiscard]]
		const String& GetResultUnit() const
		{
			return _resultUnit;
		}

		[[nodiscard]]
		const String& GetResultDescription() const
		{
			return _resultDescription;
		}

		[[nodiscard]]
		ResultType GetResultType() const
		{
			return _resultType;
		}

		[[nodiscard]]
		bool IsMeasured() const
		{
			return _isMeasured;
		}

		void SetMeasured(bool measured)
		{
			_isMeasured = measured;
		}

		TaskInfo(String name,
			String description,
			String scoreUnit,
			String scoreDescription,
			ResultType scoreType,
			bool isMeasured);

		TaskInfo(String name,
				String description,
				String scoreUnit,
				String scoreDescription,
				ResultType resultType);
		TaskInfo(const TaskInfo&) = default;
		TaskInfo(TaskInfo&&) = default;
		~TaskInfo() = default;
	 private:
		String _name;
		String _description;
		String _resultUnit;
		String _resultDescription;
		ResultType _resultType;
		bool _isMeasured;

		friend class Task;
	};

} // Elpida

#endif //ELPIDA_TASKINFO_HPP
