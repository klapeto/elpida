//
// Created by klapeto on 14/3/2023.
//

#include "Elpida/Core/MemoryInfo.hpp"

namespace Elpida
{
	MemoryInfo::MemoryInfo(Size totalSize, Size pageSize) : _totalSize(totalSize), _pageSize(pageSize)
	{
	}
	Size MemoryInfo::GetTotalSize() const
	{
		return _totalSize;
	}
	Size MemoryInfo::GetPageSize() const
	{
		return _pageSize;
	}
} // Elpida