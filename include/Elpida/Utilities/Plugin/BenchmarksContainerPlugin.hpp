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

//
// Created by klapeto on 12/4/20.
//

#ifndef INCLUDE_ELPIDA_UTILITIES_PLUGIN_BENCHMARKSCONTAINERPLUGIN_HPP
#define INCLUDE_ELPIDA_UTILITIES_PLUGIN_BENCHMARKSCONTAINERPLUGIN_HPP

#include <vector>
#include "Elpida/Utilities/Plugin/Plugin.hpp"

namespace Elpida
{

	template<typename T>
	class BenchmarksContainerPlugin : public Plugin<std::vector<T*>>
	{
	public:

		using Factory = BenchmarksContainerPlugin<T>* (*)();

		const std::vector<T*>& getUnderlyingData() const override
		{
			return _data;
		}

		void add(T* data)
		{
			_data.push_back(data);
		}

		BenchmarksContainerPlugin() = default;
		virtual ~BenchmarksContainerPlugin()
		{
			for (auto ptr : _data)
			{
				delete ptr;
			}
		}
	private:
		std::vector<T*> _data;
	};
}

#endif //INCLUDE_ELPIDA_UTILITIES_PLUGIN_BENCHMARKSCONTAINERPLUGIN_HPP
