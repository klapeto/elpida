//
// Created by klapeto on 29/11/2023.
//

#ifndef ELPIDA_INCLUDE_ELPIDA_CORE_DEFAULTALLOCATORFACTORY_HPP
#define ELPIDA_INCLUDE_ELPIDA_CORE_DEFAULTALLOCATORFACTORY_HPP

#include "AllocatorFactory.hpp"
namespace Elpida
{

	class DefaultAllocatorFactory: public AllocatorFactory
	{
	public:
		[[nodiscard]]
		Vector<SharedPtr<Allocator>> Create(const Vector<Ref<const ProcessingUnitNode>>& processors) const override;
	};

} // Elpida

#endif //ELPIDA_INCLUDE_ELPIDA_CORE_DEFAULTALLOCATORFACTORY_HPP
