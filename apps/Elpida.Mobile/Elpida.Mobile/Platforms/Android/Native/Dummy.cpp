/*
*  Copyright (c) 2024-2026  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 4/10/26.
//

#include <Elpida/Core/ModuleExports.hpp>

ELPIDA_CREATE_BENCHMARK_GROUP_FUNC();

extern "C" std::unique_ptr<Elpida::BenchmarkGroup> DoCreateBenchmarkGroup()
{
	// Trick to force resolve all linked libraries
	return ELPIDA_CREATE_BENCHMARK_GROUP_NAME();
}
