//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Result/TaskMetrics.hpp"

namespace Elpida
{

	TaskMetrics::TaskMetrics(const Elpida::Duration& duration, size_t resultValue)
		: _duration(duration), _resultValue(resultValue)
	{

	}

}
