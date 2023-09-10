//
// Created by klapeto on 21/3/2023.
//

#include "OverheadsModel.hpp"

namespace Elpida::Application
{
	OverheadsModel::OverheadsModel(const Duration& nowOverhead,
			const Duration& loopOverhead,
			const Duration& virtualCallOverhead)
			: _nowOverhead(nowOverhead), _loopOverhead(loopOverhead), _virtualCallOverhead(virtualCallOverhead)
	{
	}

	const Duration& OverheadsModel::GetNowOverhead() const
	{
		return _nowOverhead;
	}

	const Duration& OverheadsModel::GetLoopOverhead() const
	{
		return _loopOverhead;
	}

	const Duration& OverheadsModel::GetVirtualCallOverhead() const
	{
		return _virtualCallOverhead;
	}

} // Application