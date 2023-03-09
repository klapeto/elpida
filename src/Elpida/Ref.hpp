//
// Created by klapeto on 7/3/2023.
//

#ifndef ELPIDA_REF_HPP_
#define ELPIDA_REF_HPP_

#include <functional>

namespace Elpida
{
	template<typename T>
	using Ref = std::reference_wrapper<T>;
}

#endif //ELPIDA_REF_HPP_
