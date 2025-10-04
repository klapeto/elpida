/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKINFO_HPP
#define ELPIDA_BENCHMARKINFO_HPP

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ResultType.hpp"
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
		const String& GetResultUnit() const
		{
			return _resultUnit;
		}

		[[nodiscard]]
		ResultType GetResultType() const
		{
			return _resultType;
		}

		BenchmarkInfo(
				String name,
				String description,
				String resultUnit,
				ResultType resultType,
				Vector<TaskInfo> taskInfos)
				:_taskInfos(std::move(taskInfos)),
				 _name(std::move(name)),
				 _description(std::move(description)),
				 _resultUnit(std::move(resultUnit)),
				 _resultType(resultType)
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
		String _resultUnit;
		ResultType _resultType;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKINFO_HPP
