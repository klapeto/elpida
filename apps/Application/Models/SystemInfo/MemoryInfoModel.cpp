//
// Created by klapeto on 21/3/2023.
//

#include "MemoryInfoModel.hpp"

namespace Elpida::Application
{
	MemoryInfoModel::MemoryInfoModel(size_t totalSize, size_t pageSize)
			: _totalSize(totalSize), _pageSize(pageSize)
	{
	}

	std::size_t MemoryInfoModel::GetTotalSize() const
	{
		return _totalSize;
	}

	std::size_t MemoryInfoModel::GetPageSize() const
	{
		return _pageSize;
	}
} // Application