//
// Created by klapeto on 21/3/2023.
//

#include "TimingModel.hpp"
#include "Elpida/Core/Iterations.hpp"
#include "Elpida/Core/TimingInfo.hpp"

namespace Elpida::Application
{
	TimingModel::TimingModel(const Duration& nowOverhead,
		const Duration& loopOverhead,
		const Duration& virtualCallOverhead,
		Iterations iterationsPerSecond,
		const Duration& stableTime,
		TimingStability timingStability)
		: _nowOverhead(nowOverhead),
		  _loopOverhead(loopOverhead),
		  _virtualCallOverhead(virtualCallOverhead),
		  _stableTime(stableTime),
		  _iterationsPerSecond(iterationsPerSecond),
		  _timingStability(timingStability)
	{
	}

	const Duration& TimingModel::GetNowOverhead() const
	{
		return _nowOverhead;
	}

	const Duration& TimingModel::GetLoopOverhead() const
	{
		return _loopOverhead;
	}

	const Duration& TimingModel::GetVirtualCallOverhead() const
	{
		return _virtualCallOverhead;
	}

	const Duration& TimingModel::GetStableTime() const
	{
		return _stableTime;
	}

	TimingStability TimingModel::GetTimingStability() const
	{
		return _timingStability;
	}

	Size TimingModel::GetIterationsPerSecond() const
	{
		return _iterationsPerSecond;
	}

} // Application