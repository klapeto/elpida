//
// Created by klapeto on 5/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_MEMORYPAGETYPE_HPP_
#define ELPIDA_TOPOLOGY_MEMORYPAGETYPE_HPP_

#include "Elpida/Core/Size.hpp"

namespace Elpida
{
	class MemoryPageType final
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

#endif //ELPIDA_TOPOLOGY_MEMORYPAGETYPE_HPP_
