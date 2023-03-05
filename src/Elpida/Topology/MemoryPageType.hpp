//
// Created by klapeto on 5/3/2023.
//

#ifndef _MEMORYPAGETYPE_HPP_
#define _MEMORYPAGETYPE_HPP_

#include <cstdlib>

namespace Elpida
{

	class MemoryPageType
	{
	 public:
		[[nodiscard]]
		std::size_t GetPageSize() const;

		[[nodiscard]]
		std::size_t GetPageCount() const;

		MemoryPageType(size_t pageSize, size_t pageCount);
		MemoryPageType(const MemoryPageType&) = delete;
		MemoryPageType(MemoryPageType&&) noexcept = default;
		MemoryPageType& operator=(const MemoryPageType&) = delete;
		MemoryPageType& operator=(MemoryPageType&&) noexcept = default;
		~MemoryPageType() = default;
	 private:
		std::size_t _pageSize;
		std::size_t _pageCount;
	};

} // Elpida

#endif //_MEMORYPAGETYPE_HPP_
