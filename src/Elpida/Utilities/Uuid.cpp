//
// Created by klapeto on 17/4/20.
//

#include "Elpida/Utilities/Uuid.hpp"
#include "Elpida/Config.hpp"
#ifdef ELPIDA_LINUX
#include <fstream>
#else
#include <Windows.h>
#include <sstream>
#include <iomanip>
#endif

namespace Elpida
{

	std::string Uuid::create()
	{
#if defined(ELPIDA_LINUX)
		std::fstream file("/proc/sys/kernel/random/uuid", std::fstream::in);
		file.exceptions(std::ios_base::failbit);
		std::string uuid;
		if (file.good())
		{
			file >> uuid;
		}
		return uuid;
#elif defined(ELPIDA_WINDOWS)
		GUID uuid;
		auto status = CoCreateGuid(&uuid);
		std::ostringstream stream;
		stream << std::hex << std::setfill ('0') << std::setw(sizeof(UUID::Data1) * 2) << uuid.Data1 << '-'
			<< std::hex << std::setfill ('0') << std::setw(sizeof(UUID::Data2) * 2) << uuid.Data2 << '-'
			<< std::hex << std::setfill ('0') << std::setw(sizeof(UUID::Data3) * 2) << uuid.Data3;
		for (auto c: uuid.Data4)
		{
			stream << '-' << std::hex
					<< std::setfill ('0')
					<< std::setw(sizeof(UUID::Data4[0]) * 2)
					<< (int)c; // (int) casting is needed for not treating as printable char

		}
		return stream.str();
#else
#error "Platform not supported"
#endif
	}
}