//
// Created by klapeto on 7/3/2023.
//

#ifndef _REF_HPP_
#define _REF_HPP_

#include <functional>

namespace Elpida
{
	template<typename T>
	using Ref = std::reference_wrapper<T>;
}

#endif //_REF_HPP_
