//
// Created by klapeto on 29/11/2023.
//

#ifndef ELPIDA_INCLUDE_ELPIDA_CORE_DEFAULTALLOCATORFACTORY_HPP
#define ELPIDA_INCLUDE_ELPIDA_CORE_DEFAULTALLOCATORFACTORY_HPP

#include "AllocatorFactory.hpp"
namespace Elpida
{

	class DefaultAllocatorFactory final : public AllocatorFactory
	{
	public:
		[[nodiscard]]
		Vector<SharedPtr<Allocator>> Create(const Vector<Ref<const ProcessingUnitNode>>& processors) const override;

		DefaultAllocatorFactory() = default;
		~DefaultAllocatorFactory() override = default;
	};

} // Elpida

#endif //ELPIDA_INCLUDE_ELPIDA_CORE_DEFAULTALLOCATORFACTORY_HPP
