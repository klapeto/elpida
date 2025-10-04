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
