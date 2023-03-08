//
// Created by klapeto on 5/3/2023.
//

#ifndef _MEMORYPAGETYPE_HPP_
#define _MEMORYPAGETYPE_HPP_

#include "Elpida/Size.hpp"

namespace Elpida
{
	class MemoryPageType
	{
	 public:
		[[nodiscard]]
		Size GetPageSize() const;

		[[nodiscard]]
		Size GetPageCount() const;

		MemoryPageType(Size pageSize, Size pageCount);
		MemoryPageType(const MemoryPageType&) = delete;
		MemoryPageType(MemoryPageType&&) noexcept = default;
		MemoryPageType& operator=(const MemoryPageType&) = delete;
		MemoryPageType& operator=(MemoryPageType&&) noexcept = default;
		~MemoryPageType() = default;
	 private:
		Size _pageSize;
		Size _pageCount;
	};

} // Elpida

#endif //_MEMORYPAGETYPE_HPP_
