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