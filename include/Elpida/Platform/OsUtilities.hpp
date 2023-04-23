//
// Created by klapeto on 12/3/2023.
//

#ifndef ELPIDA_OSUTILITIES_HPP_
#define ELPIDA_OSUTILITIES_HPP_

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"

namespace Elpida
{

	class OsUtilities
	{
	 public:
		static String GetLastErrorString();
		static unsigned int GetNumaNodeIdForProcessor(unsigned int processorId);
		static String ExecuteProcess(const String& path, const Vector<String>& args);
	};

} // Elpida

#endif //ELPIDA_OSUTILITIES_HPP_
