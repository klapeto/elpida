//
// Created by klapeto on 7/3/2023.
//

#ifndef _UNIQUEPTR_HPP_
#define _UNIQUEPTR_HPP_

#include <memory>

namespace Elpida
{
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
}

#endif //_UNIQUEPTR_HPP_
