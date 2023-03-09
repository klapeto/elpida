//
// Created by klapeto on 7/3/2023.
//

#ifndef ELPIDA_UNIQUEPTR_HPP_
#define ELPIDA_UNIQUEPTR_HPP_

#include <memory>

namespace Elpida
{
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
}

#endif //ELPIDA_UNIQUEPTR_HPP_
