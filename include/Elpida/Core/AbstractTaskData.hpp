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
// Created by klapeto on 7/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_ABSTRACTTASKDATA_HPP_
#define ELPIDA_TOPOLOGY_ABSTRACTTASKDATA_HPP_

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/SharedPtr.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Size.hpp"

namespace Elpida
{
	class Allocator;

	class AbstractTaskData
	{
	 public:
		[[nodiscard]]
		virtual unsigned char* GetData() const = 0;

		[[nodiscard]]
		virtual Size GetSize() const = 0;

		virtual void Allocate(Size size) = 0;

		virtual void ReAllocate(Size size) = 0;

		virtual void Deallocate() = 0;

		[[nodiscard]]
		virtual SharedPtr<Allocator> GetAllocator() const = 0;

		[[nodiscard]]
		virtual Vector<SharedPtr<AbstractTaskData>>
		Copy(const Vector<SharedPtr<Allocator>>& targetAllocators) const = 0;

		[[nodiscard]]
		virtual Vector<SharedPtr<AbstractTaskData>>
		Split(const Vector<SharedPtr<Allocator>>& targetAllocators) const = 0;

		virtual void Merge(const Vector<SharedPtr<AbstractTaskData>>& data) = 0;

		virtual ~AbstractTaskData() = default;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_ABSTRACTTASKDATA_HPP_
