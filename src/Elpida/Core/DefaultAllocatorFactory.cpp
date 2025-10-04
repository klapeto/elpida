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
// Created by klapeto on 29/11/2023.
//

#include "Elpida/Core/DefaultAllocatorFactory.hpp"
#include "Elpida/Core/DefaultAllocator.hpp"
#include <cstddef>

namespace Elpida
{
	Vector<SharedPtr<Allocator>> DefaultAllocatorFactory::Create(const Vector<Ref<const ProcessingUnitNode>>& processors) const
	{
		Vector<SharedPtr<Allocator>> returnAllocators;
		returnAllocators.reserve(processors.size());
		for (std::size_t i = 0; i < processors.size(); ++i)
		{
			returnAllocators.push_back(std::make_shared<DefaultAllocator>());
		}
		return returnAllocators;
	}
} // Elpida