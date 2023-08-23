//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_TASKINFO_HPP
#define ELPIDA_TASKINFO_HPP

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ScoreComparisonType.hpp"
#include "Elpida/Core/ScoreType.hpp"

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
		const String& GetScoreUnit() const
		{
			return _scoreUnit;
		}

		[[nodiscard]]
		const String& GetScoreDescription() const
		{
			return _scoreDescription;
		}

		[[nodiscard]]
		ScoreType GetScoreType() const
		{
			return _scoreType;
		}

		TaskInfo(String name,
			String description,
			String scoreUnit,
			String scoreDescription,
			ScoreType scoreType);
		TaskInfo(const TaskInfo&) = default;
		TaskInfo(TaskInfo&&) = default;
		~TaskInfo() = default;
	 private:
		String _name;
		String _description;
		String _scoreUnit;
		String _scoreDescription;
		ScoreType _scoreType;
	};

} // Elpida

#endif //ELPIDA_TASKINFO_HPP
