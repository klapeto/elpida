//
// Created by klapeto on 11/4/2023.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "RSADecryptionBenchmark.hpp"
#include "RSAEncryptionBenchmark.hpp"
#include "AESDecryptionBenchmark.hpp"
#include "AESEncryptionBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.emplace_back(new RSAEncryptionBenchmark());
	vec.emplace_back(new RSADecryptionBenchmark());
	vec.emplace_back(new AESEncryptionBenchmark());
	vec.emplace_back(new AESDecryptionBenchmark());

	return std::make_unique<BenchmarkGroup>("Encryption benchmarks", std::move(vec));
}