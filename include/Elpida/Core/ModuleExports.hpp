//
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_MODULEEXPORTS_HPP_
#define ELPIDA_MODULEEXPORTS_HPP_

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/UniquePtr.hpp"


namespace Elpida
{
	class BenchmarkGroup;

	//typedef BenchmarkGroup* BenchmarkGroupPtr;
	using BenchmarkGroupPtr = BenchmarkGroup*;

	using BenchmarkGroupCreator = BenchmarkGroup* (*)();
	using BenchmarkGroupDestructor = void (*)(BenchmarkGroup*);

	constexpr inline const char* BenchmarkGroupCreatorName = "CreateBenchmarkGroup";
	constexpr inline const char* BenchmarkGroupDestructorName = "DestroyBenchmarkGroup";

	using BenchmarkGroupUniquePtr = UniquePtr<BenchmarkGroup, BenchmarkGroupDestructor>;
}


#define ELPIDA_CREATE_BENCHMARK_GROUP_NAME CreateBenchmarkGroup
#define ELPIDA_CREATE_BENCHMARK_GROUP_FUNC ELPIDA_EXPORT ELPIDA_STDCALL Elpida::BenchmarkGroupPtr ELPIDA_CREATE_BENCHMARK_GROUP_NAME
#define ELPIDA_CREATE_BENCHMARK_GROUP_DECL extern "C" ELPIDA_CREATE_BENCHMARK_GROUP_FUNC()

#define ELPIDA_DESTROY_BENCHMARK_GROUP_NAME DestroyBenchmarkGroup
#define ELPIDA_DESTROY_BENCHMARK_GROUP_FUNC ELPIDA_EXPORT ELPIDA_STDCALL void ELPIDA_DESTROY_BENCHMARK_GROUP_NAME
#define ELPIDA_DESTROY_BENCHMARK_GROUP_DECL extern "C" ELPIDA_DESTROY_BENCHMARK_GROUP_FUNC(Elpida::BenchmarkGroupPtr group)


#endif //ELPIDA_MODULEEXPORTS_HPP_
