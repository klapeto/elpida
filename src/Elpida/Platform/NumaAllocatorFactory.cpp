//
// Created by klapeto on 29/11/2023.
//

#include "Elpida/Platform/NumaAllocatorFactory.hpp"
#include "Elpida/Platform/NumaAllocator.hpp"

namespace Elpida
{
	Vector<SharedPtr<Allocator>> NumaAllocatorFactory::Create(const Vector<Ref<const ProcessingUnitNode>>& processors) const
	{
		Vector<SharedPtr<Allocator>> returnAllocators;
		returnAllocators.reserve(processors.size());
		for (auto& processor: processors)
		{
			returnAllocators.push_back(std::make_shared<NumaAllocator>(processor));
		}
		return returnAllocators;
	}
} // Elpida