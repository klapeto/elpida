//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKINFO_HPP
#define ELPIDA_BENCHMARKINFO_HPP

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ScoreType.hpp"
#include "Elpida/Core/TaskInfo.hpp"

namespace Elpida
{
	class BenchmarkInfo final
	{
	 public:

		[[nodiscard]]
		const Vector<TaskInfo>& GetTaskInfos() const
		{
			return _taskInfos;
		}

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

		BenchmarkInfo(String name,
			String description,
			String scoreUnit,
			String scoreDescription,
			const Vector<TaskInfo>& taskInfos,
			String uuid = String())
			: _taskInfos(taskInfos),
			  _name(std::move(name)),
			  _description(std::move(description)),
			  _scoreUnit(std::move(scoreUnit)),
			  _scoreDescription(std::move(scoreDescription)),
			  _uuid(std::move(uuid))
		{
		}

		BenchmarkInfo(const BenchmarkInfo&) = default;
		BenchmarkInfo(BenchmarkInfo&&) noexcept = default;
		BenchmarkInfo& operator=(const BenchmarkInfo&) = default;
		BenchmarkInfo& operator=(BenchmarkInfo&&) noexcept = default;
		~BenchmarkInfo() = default;
	 private:
		Vector<TaskInfo> _taskInfos;
		String _name;
		String _description;
		String _scoreUnit;
		String _scoreDescription;
		String _uuid;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKINFO_HPP
