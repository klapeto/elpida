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

		[[nodiscard]]
		const std::vector<TaskInfo>& GetTaskInfos() const
		{
			return _taskInfos;
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

		BenchmarkInfo(std::string name,
			std::string description,
			std::string scoreUnit,
			std::string scoreDescription,
			const std::vector<TaskInfo>& taskInfos)
			: _taskInfos(taskInfos),
			  _name(std::move(name)),
			  _description(std::move(description)),
			  _scoreUnit(std::move(scoreUnit)),
			  _scoreDescription(std::move(scoreDescription))
		{
		}
		~BenchmarkInfo() = default;
	 private:
		std::vector<TaskInfo> _taskInfos;
		std::string _name;
		std::string _description;
		std::string _scoreUnit;
		std::string _scoreDescription;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKINFO_HPP
