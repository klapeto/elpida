//
// Created by klapeto on 21/3/2023.
//

#include "TimingModel.hpp"
#include "Elpida/Core/TimingInfo.hpp"

namespace Elpida::Application
{
	TimingModel::TimingModel(const Duration& nowOverhead,
		const Duration& loopOverhead,
		Iterations iterationsPerSecond)
		: _nowOverhead(nowOverhead),
		  _loopOverhead(loopOverhead),
		  _iterationsPerSecond(iterationsPerSecond)
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

	Iterations TimingModel::GetIterationsPerSecond() const
	{
		return _iterationsPerSecond;
	}

} // Application