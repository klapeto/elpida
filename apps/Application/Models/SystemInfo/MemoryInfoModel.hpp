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
// Created by klapeto on 21/3/2023.
//

#ifndef ELPIDA_MEMORYINFOMODEL_HPP
#define ELPIDA_MEMORYINFOMODEL_HPP

#include "Models/Abstractions/Model.hpp"

#include <cstdlib>

namespace Elpida::Application
{

	class MemoryInfoModel : public Model
	{
	public:

		[[nodiscard]]
		std::size_t GetTotalSize() const;

		[[nodiscard]]
		std::size_t GetPageSize() const;

		MemoryInfoModel() = default;
		MemoryInfoModel(std::size_t totalSize, std::size_t pageSize);
		MemoryInfoModel(MemoryInfoModel&&) noexcept = default;
		MemoryInfoModel& operator=(MemoryInfoModel&&) noexcept = default;
		MemoryInfoModel(const MemoryInfoModel&) = delete;
		MemoryInfoModel& operator=(const MemoryInfoModel&) = delete;
		~MemoryInfoModel() override = default;
	private:
		std::size_t _totalSize;
		std::size_t _pageSize;
	};

} // Application

#endif //ELPIDA_MEMORYINFOMODEL_HPP
