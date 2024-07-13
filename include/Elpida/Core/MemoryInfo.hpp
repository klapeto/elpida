//
// Created by klapeto on 14/3/2023.
//

#ifndef ELPIDA_MEMORYINFO_HPP_
#define ELPIDA_MEMORYINFO_HPP_

#include "Elpida/Core/Size.hpp"

namespace Elpida
{

	class MemoryInfo final
	{
	 public:
		[[nodiscard]]
		Size GetTotalSize() const;

		[[nodiscard]]
		Size GetPageSize() const;

		MemoryInfo(Size totalSize, Size pageSize);
		MemoryInfo(const MemoryInfo&) = default;
		MemoryInfo(MemoryInfo&&) noexcept = default;
		MemoryInfo& operator=(const MemoryInfo&) = default;
		MemoryInfo& operator=(MemoryInfo&&) noexcept = default;
		~MemoryInfo() = default;
	 private:
		Size _totalSize;
		Size _pageSize;
	};

} // Elpida

#endif //ELPIDA_MEMORYINFO_HPP_
