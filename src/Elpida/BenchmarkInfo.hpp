//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKINFO_HPP
#define ELPIDA_BENCHMARKINFO_HPP

#include <string>
#include <utility>
#include <vector>
#include "Elpida/ScoreType.hpp"
#include "Elpida/TaskInfo.hpp"

namespace Elpida
{
	class BenchmarkInfo
	{
	 public:

		const std::vector<TaskInfo>& GetTaskInfos() const
		{
			return _taskInfos;
		}

		const std::string& GetName() const
		{
			return _name;
		}

		const std::string& GetDescription() const
		{
			return _description;
		}

		const std::string& GetScoreUnit() const
		{
			return _scoreUnit;
		}

		const std::string& GetScoreDescription() const
		{
			return _scoreDescription;
		}

		ScoreType GetScoreType() const
		{
			return _scoreType;
		}

		BenchmarkInfo(std::string name,
			std::string description,
			std::string scoreUnit,
			std::string scoreDescription,
			ScoreType scoreType,
			const std::vector<TaskInfo>& taskInfos)
			: _taskInfos(taskInfos),
			  _name(std::move(name)),
			  _description(std::move(description)),
			  _scoreUnit(std::move(scoreUnit)),
			  _scoreDescription(std::move(scoreDescription)),
			  _scoreType(scoreType)
		{
		}
		~BenchmarkInfo() = default;
	 private:
		std::vector<TaskInfo> _taskInfos;
		std::string _name;
		std::string _description;
		std::string _scoreUnit;
		std::string _scoreDescription;
		ScoreType _scoreType;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKINFO_HPP
