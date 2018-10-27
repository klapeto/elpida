/**************************************************************************
*   Elpida - Benchmark library
*   
*   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include "ElpidaManager.hpp"
#include "Elpida/Exceptions/IOException.hpp"
#include <iostream>

namespace Elpida
{
	ElpidaManager::ElpidaManager()
	{
		std::cerr.rdbuf(_log.rdbuf());
		std::cout.rdbuf(_log.rdbuf());
	}

	ElpidaManager::~ElpidaManager()
	{
		std::cout.rdbuf(nullptr);
		std::cerr.rdbuf(nullptr);
		_log.flush();
	}

	void ElpidaManager::reloadPlugins()
	{
		_batchLoader.loadFromFolder(_batchesDirectory);
	}
}

