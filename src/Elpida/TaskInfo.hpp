//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_TASKINFO_HPP
#define ELPIDA_TASKINFO_HPP

#include <string>

#include "Elpida/ScoreComparisonType.hpp"
#include "Elpida/ScoreType.hpp"
#include "Elpida/DataInfo.hpp"

namespace Elpida
{

	class TaskInfo
	{
	 public:
		[[nodiscard]]
		const DataInfo& GetInputInfo() const
		{
			return _inputInfo;
		}

		[[nodiscard]]
		const DataInfo& GetOutputInfo() const
		{
			return _outputInfo;
		}

		[[nodiscard]]
		const std::string& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const std::string& GetDescription() const
		{
			return _description;
		}

		[[nodiscard]]
		const std::string& GetScoreUnit() const
		{
			return _scoreUnit;
		}

		[[nodiscard]]
		const std::string& GetScoreDescription() const
		{
			return _scoreDescription;
		}

		[[nodiscard]]
		ScoreType GetScoreType() const
		{
			return _scoreType;
		}

		TaskInfo(std::string name,
			std::string description,
			std::string scoreUnit,
			std::string scoreDescription,
			ScoreType scoreType,
			DataInfo inputInfo,
			DataInfo outputInfo);

		TaskInfo(const TaskInfo&) = default;

		TaskInfo(TaskInfo&&) = default;

		~TaskInfo() = default;

	 private:
		DataInfo _inputInfo;
		DataInfo _outputInfo;
		std::string _name;
		std::string _description;
		std::string _scoreUnit;
		std::string _scoreDescription;
		ScoreType _scoreType;
	};

} // Elpida

#endif //ELPIDA_TASKINFO_HPP
