/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2022  Ioannis Panagiotopoulos
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
// Created by klapeto on 11/12/2022.
//

#ifndef ELPIDA_BENCHMARKLOADER_HPP
#define ELPIDA_BENCHMARKLOADER_HPP

#include <string>
#include <vector>
#include <memory>

#include "Elpida/Utilities/SharedLibrary.hpp"
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include "Elpida/Utilities/SharedLibraryLoader.hpp"

namespace Elpida
{
	template<typename T>
	class BenchmarksContainerPlugin;

	class Logger;

	class BenchmarkLoader
	{
	public:
		const std::vector<std::unique_ptr<Benchmark>>& load(const std::string& path);

		explicit BenchmarkLoader(const ServiceProvider& serviceProvider)
				: _serviceProvider(serviceProvider)
		{

		}

		~BenchmarkLoader();
	private:
		std::vector<std::tuple<std::reference_wrapper<const SharedLibrary>, BenchmarksContainerPlugin<Benchmark>*>> _createdPlugins;
		SharedLibraryLoader _libraryLoader;
		const ServiceProvider& _serviceProvider;

		const std::vector<std::unique_ptr<Benchmark>>& load(const SharedLibrary& library);
	};

} // Elpida

#endif //ELPIDA_BENCHMARKLOADER_HPP
