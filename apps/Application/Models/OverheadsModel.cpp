//
// Created by klapeto on 21/3/2023.
//

#include "OverheadsModel.hpp"

namespace Elpida::Application
{
	OverheadsModel::OverheadsModel(const Time& nowOverhead,
			const Time& loopOverhead,
			const Time& virtualCallOverhead)
			: _nowOverhead(nowOverhead), _loopOverhead(loopOverhead), _virtualCallOverhead(virtualCallOverhead)
	{
	}

	const Time& OverheadsModel::GetNowOverhead() const
	{
		return _nowOverhead;
	}

	const Time& OverheadsModel::GetLoopOverhead() const
	{
		return _loopOverhead;
	}

	const Time& OverheadsModel::GetVirtualCallOverhead() const
	{
		return _virtualCallOverhead;
	}

} // Application