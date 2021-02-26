/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * Cpu.cpp
 *
 *  Created on: 10 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/SystemInfo/CpuInfo.hpp"
#include "Elpida/SystemInfo/X86Info.hpp"

namespace Elpida
{

	const CpuInfo& CpuInfo::get()
	{
		static
#if __x86_64__ || _M_X64
		X86Info
#else
#error "Unsupported Platform"
#endif
		cpuInfo;

		return cpuInfo;
	}
} /* namespace Elpida */

