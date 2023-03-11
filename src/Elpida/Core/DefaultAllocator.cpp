//
// Created by klapeto on 11/3/2023.
//

#include "Elpida/Core/DefaultAllocator.hpp"

namespace Elpida
{
	void* DefaultAllocator::Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const
	{
		return malloc(size);
	}

	void DefaultAllocator::Deallocate(void* ptr, Size size) const
	{
		free(ptr);
	}
} // Elpida