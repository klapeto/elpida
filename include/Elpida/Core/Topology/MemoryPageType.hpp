/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
