//
// Created by klapeto on 28/11/2023.
//

#ifndef ELPIDA_INCLUDE_ELPIDA_CORE_ALLOCATORFACTORY_HPP
#define ELPIDA_INCLUDE_ELPIDA_CORE_ALLOCATORFACTORY_HPP

#include "Allocator.hpp"
#include "Ref.hpp"
#include "Vector.hpp"
#include "SharedPtr.hpp"

namespace Elpida
{

	class AllocatorFactory
	{
	public:
		[[nodiscard]] 
		virtual Vector<SharedPtr<Allocator>> Create(const Vector<Ref<const ProcessingUnitNode>>& processors) const = 0;
	};

} // Elpida

#endif //ELPIDA_INCLUDE_ELPIDA_CORE_ALLOCATORFACTORY_HPP
