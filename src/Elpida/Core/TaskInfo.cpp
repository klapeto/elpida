//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/TaskInfo.hpp"

#include <utility>

namespace Elpida
{
	TaskInfo::TaskInfo(String name,
			String description,
			String scoreUnit,
			String scoreDescription,
			ResultType resultType) :_name(std::move(name)),
									_description(std::move(description)),
									_resultUnit(std::move(scoreUnit)),
									_resultDescription(
									   std::move(scoreDescription)),
									_resultType(resultType),
									_isMeasured(false)
	{
	}

	TaskInfo::TaskInfo(String name,
			String description,
			String scoreUnit,
			String scoreDescription,
			ResultType scoreType,
			bool isMeasured) :_name(std::move(name)),
							  _description(std::move(description)),
							  _resultUnit(std::move(scoreUnit)),
							  _resultDescription(
									   std::move(scoreDescription)),
							  _resultType(scoreType),
							  _isMeasured(isMeasured)
	{
	}
} // Elpida