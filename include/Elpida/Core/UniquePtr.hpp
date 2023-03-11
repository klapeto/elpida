//
// Created by klapeto on 7/3/2023.
//

#ifndef ELPIDA_UNIQUEPTR_HPP_
#define ELPIDA_UNIQUEPTR_HPP_

#include <memory>

namespace Elpida
{
	template<typename T, typename TDeleter = std::default_delete<T>>
	using UniquePtr = std::unique_ptr<T, TDeleter>;
}

#endif //ELPIDA_UNIQUEPTR_HPP_
