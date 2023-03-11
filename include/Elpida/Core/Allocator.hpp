//
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_ALLOCATOR_HPP_
#define ELPIDA_ALLOCATOR_HPP_

#include "Elpida/Core/Size.hpp"

namespace Elpida
{
	class ProcessingUnitNode;

	class Allocator
	{
	 public:
		[[nodiscard]]
		virtual void* Allocate(const ProcessingUnitNode& targetProcessingUnit, Size size) const = 0;
		virtual void Deallocate(void* ptr, Size size) const = 0;

		Allocator() = default;
		virtual ~Allocator() = default;
	};

} // Elpida

#endif //ELPIDA_ALLOCATOR_HPP_
