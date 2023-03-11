//
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_DEFAULTALLOCATOR_HPP_
#define ELPIDA_DEFAULTALLOCATOR_HPP_

#include "Elpida/Core/Allocator.hpp"

namespace Elpida
{
	class DefaultAllocator final : public Allocator
	{
	 public:
		[[nodiscard]]
		void* Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const final;
		void Deallocate(void* ptr, Size size) const final;

		DefaultAllocator() = default;
		~DefaultAllocator() final = default;
	};

} // Elpida

#endif //ELPIDA_DEFAULTALLOCATOR_HPP_
