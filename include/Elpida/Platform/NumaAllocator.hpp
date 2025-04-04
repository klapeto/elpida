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
		void* Allocate(Size size) override;
		void Deallocate(void* ptr, Size size) noexcept override;
		void* Reallocate(void* ptr, Size oldSize, Size newSize) override;

		explicit NumaAllocator(const ProcessingUnitNode& targetProcessingUnit);
		~NumaAllocator() final = default;
	private:
		const ProcessingUnitNode& _targetProcessingUnit;
	};

} // Elpida

#endif //ELPIDA_NUMAALLOCATOR_HPP_
