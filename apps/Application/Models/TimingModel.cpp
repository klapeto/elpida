//
// Created by klapeto on 21/3/2023.
//

#include "TimingModel.hpp"

namespace Elpida::Application
{
	TimingModel::TimingModel(const Duration& nowOverhead,
		const Duration& loopOverhead,
		const Duration& virtualCallOverhead,
		Size bogusIps,
		const Duration& stableTime,
		bool systemStable)
		: _nowOverhead(nowOverhead),
		  _loopOverhead(loopOverhead),
		  _virtualCallOverhead(virtualCallOverhead),
		  _stableTime(stableTime),
		  _bogusIps(bogusIps),
		  _systemStable(systemStable)
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

	bool TimingModel::IsSystemStable() const
	{
		return _systemStable;
	}

	Size TimingModel::GetBogusIps() const
	{
		return _bogusIps;
	}

} // Application