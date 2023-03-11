//
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_NUMAALLOCATOR_HPP_
#define ELPIDA_NUMAALLOCATOR_HPP_

#include "Elpida/Core/Allocator.hpp"

namespace Elpida
{

	class NumaAllocator final : public Allocator
	{
	 public:
		[[nodiscard]]
		void* Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const = 0;
		void Deallocate(void* ptr, Size size) const = 0;

		NumaAllocator() = default;
		~NumaAllocator() final = default;
	};

} // Elpida

#endif //ELPIDA_NUMAALLOCATOR_HPP_
