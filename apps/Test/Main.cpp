//
// Created by klapeto on 27/2/2023.
//

#include <cstdlib>

#include "Elpida/Test/TestBenchmark.hpp"

using namespace Elpida;

int main(int argC, char* argV[])
{
	TestBenchmark benchmark;

	auto result = benchmark.Run();

	return EXIT_SUCCESS;
}