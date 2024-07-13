//
// Created by klapeto on 5/3/2023.
//

#include "Elpida/Core/Topology/MemoryPageType.hpp"

namespace Elpida
{
	MemoryPageType::MemoryPageType(Size pageSize, Size pageCount)
		: _pageSize(pageSize), _pageCount(pageCount)
	{
	}

	Size MemoryPageType::GetPageSize() const
	{
		return _pageSize;
	}

	Size MemoryPageType::GetPageCount() const
	{
		return _pageCount;
	}
} // Elpida