//
// Created by klapeto on 29/11/2023.
//

#ifndef ELPIDA_SRC_ELPIDA_PLATFORM_NUMAALLOCATORFACTORY_HPP
#define ELPIDA_SRC_ELPIDA_PLATFORM_NUMAALLOCATORFACTORY_HPP

#include "Elpida/Core/AllocatorFactory.hpp"
namespace Elpida
{

	class NumaAllocatorFactory : public AllocatorFactory
	{
	public:
		[[nodiscard]]
		Vector<SharedPtr<Allocator>> Create(const Vector<Ref<const ProcessingUnitNode>>& processors) const override;

		NumaAllocatorFactory() = default;
		~NumaAllocatorFactory() override = default;
	};

} // Elpida

#endif //ELPIDA_SRC_ELPIDA_PLATFORM_NUMAALLOCATORFACTORY_HPP
