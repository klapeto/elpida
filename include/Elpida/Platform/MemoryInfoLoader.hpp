//
// Created by klapeto on 14/3/2023.
//

#ifndef ELPIDA_MEMORYINFOLOADER_HPP_
#define ELPIDA_MEMORYINFOLOADER_HPP_

#include "Elpida/Core/MemoryInfo.hpp"

namespace Elpida
{

	class MemoryInfoLoader final
	{
	 public:

		static MemoryInfo Load();

		MemoryInfoLoader() = delete;
	};

} // Elpida

#endif //ELPIDA_MEMORYINFOLOADER_HPP_
