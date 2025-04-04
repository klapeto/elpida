//
// Created by klapeto on 13/3/2023.
//

#ifndef ELPIDA_OSINFOLOADER_HPP_
#define ELPIDA_OSINFOLOADER_HPP_

#include "Elpida/Core/OsInfo.hpp"
namespace Elpida
{

	class OsInfoLoader final
	{
	 public:
		static OsInfo Load();
		OsInfoLoader() = delete;
	};

} // Elpida

#endif //ELPIDA_OSINFOLOADER_HPP_
