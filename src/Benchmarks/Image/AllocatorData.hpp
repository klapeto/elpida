//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_ALLOCATORDATA_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_ALLOCATORDATA_HPP

namespace Elpida {
	class Allocator;
	class ProcessingUnitNode;

	struct AllocatorData {
		const Allocator* allocator;
		const ProcessingUnitNode* targetProcessor;
	};

	static void* N_Allocate(std::size_t size, void* state)
	{
		auto* allocatorData = reinterpret_cast<AllocatorData*>(state);
		return allocatorData->allocator->Allocate(*allocatorData->targetProcessor, size);
	}

	static void N_Deallocate(void* ptr, std::size_t size, void* state)
	{
		auto* allocatorData = reinterpret_cast<AllocatorData*>(state);
		allocatorData->allocator->Deallocate(ptr, size);
	}

	static void* N_Reallocate(void* ptr, std::size_t oldSize, std::size_t newSize, void* state)
	{
		auto* allocatorData = reinterpret_cast<AllocatorData*>(state);
		return allocatorData->allocator->Reallocate(*allocatorData->targetProcessor, ptr, oldSize, newSize);
	}
}


#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_ALLOCATORDATA_HPP
