//
// Created by klapeto on 7/3/2023.
//

#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <unordered_map>

namespace Elpida
{
	template<typename TKey, typename TValue>
	using Map = std::unordered_map<TKey, TValue>;
}

#endif //_MAP_HPP_
