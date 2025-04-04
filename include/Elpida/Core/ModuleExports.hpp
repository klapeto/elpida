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
