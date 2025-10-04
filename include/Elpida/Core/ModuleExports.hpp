/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_MODULEEXPORTS_HPP_
#define ELPIDA_MODULEEXPORTS_HPP_

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/BenchmarkGroup.hpp"
#include <memory>

#define ELPIDA_CREATE_BENCHMARK_GROUP_NAME CreateBenchmarkGroup
#define ELPIDA_CREATE_BENCHMARK_GROUP_FUNC ELPIDA_EXPORT std::unique_ptr<Elpida::BenchmarkGroup> ELPIDA_STDCALL ELPIDA_CREATE_BENCHMARK_GROUP_NAME
#define ELPIDA_CREATE_BENCHMARK_GROUP_DECL ELPIDA_CREATE_BENCHMARK_GROUP_FUNC()

#endif //ELPIDA_MODULEEXPORTS_HPP_
