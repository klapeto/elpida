//
// Created by klapeto on 27/2/2023.
//

#include "TaskInfo.hpp"

#include <utility>

namespace Elpida
{
	TaskInfo::TaskInfo(std::string name,
		std::string description,
		std::string scoreUnit,
		std::string scoreDescription,
		ScoreType scoreType,
		DataInfo inputInfo,
		DataInfo outputInfo) : _inputInfo(std::move(inputInfo)),
							   _outputInfo(std::move(outputInfo)),
							   _name(std::move(name)),
							   _description(std::move(description)),
							   _scoreUnit(std::move(scoreUnit)),
							   _scoreDescription(
								   std::move(scoreDescription)),
							   _scoreType(scoreType)
	{
	}
} // Elpida