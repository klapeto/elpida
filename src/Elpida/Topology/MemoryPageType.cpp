//
// Created by klapeto on 5/3/2023.
//

#include "MemoryPageType.hpp"

namespace Elpida
{
	MemoryPageType::MemoryPageType(size_t pageSize, size_t pageCount) : _pageSize(pageSize), _pageCount(pageCount)
	{
	}

	std::size_t MemoryPageType::GetPageSize() const
	{
		return _pageSize;
	}

	std::size_t MemoryPageType::GetPageCount() const
	{
		return _pageCount;
	}
} // Elpida