//
// Created by klapeto on 29/11/2023.
//

#ifndef ELPIDA_INCLUDE_ELPIDA_CORE_SHAREDPTR_HPP
#define ELPIDA_INCLUDE_ELPIDA_CORE_SHAREDPTR_HPP

#include <memory>

namespace Elpida
{
	template<typename T>
	using SharedPtr = std::shared_ptr<T>;
}
#endif //ELPIDA_INCLUDE_ELPIDA_CORE_SHAREDPTR_HPP
