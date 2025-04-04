//
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_DEFAULTALLOCATOR_HPP_
#define ELPIDA_DEFAULTALLOCATOR_HPP_

#include "Elpida/Core/Allocator.hpp"
#include "Elpida/Core/Duration.hpp"

namespace Elpida
{
	class DefaultAllocator final : public Allocator
	{
	 public:
		[[nodiscard]]
		void* Allocate(Size size) final;
		void Deallocate(void* ptr, Size size) noexcept final;
		void* Reallocate(void* ptr, Size oldSize, Size newSize) override;
		DefaultAllocator() = default;
		~DefaultAllocator() final = default;
	};

} // Elpida

#endif //ELPIDA_DEFAULTALLOCATOR_HPP_
