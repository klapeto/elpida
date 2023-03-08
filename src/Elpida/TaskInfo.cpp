//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/TaskInfo.hpp"

#include <utility>

namespace Elpida
{
	TaskInfo::TaskInfo(String name,
		String description,
		String scoreUnit,
		String scoreDescription,
		ScoreType scoreType) : _name(std::move(name)),
							   _description(std::move(description)),
							   _scoreUnit(std::move(scoreUnit)),
							   _scoreDescription(
								   std::move(scoreDescription)),
							   _scoreType(scoreType)
	{
	}
} // Elpida