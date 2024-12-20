//
// Created by klapeto on 7/4/2024.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ModuleExports.hpp"

#include "ClangCompilationBenchmark.hpp"

using namespace Elpida;

extern "C" {
void LLVMInitializeX86Target();
void LLVMInitializeX86TargetInfo();
void LLVMInitializeX86TargetMC();
void LLVMInitializeX86AsmPrinter();
}

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	LLVMInitializeX86TargetInfo();
	LLVMInitializeX86Target();
	LLVMInitializeX86TargetMC();
	LLVMInitializeX86AsmPrinter();

	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<ClangCompilationBenchmark>());

	return std::make_unique<BenchmarkGroup>("Compilation benchmarks", std::move(vec));
}