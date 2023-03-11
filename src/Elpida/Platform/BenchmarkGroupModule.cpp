//
// Created by klapeto on 11/3/2023.
//

#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	const BenchmarkGroup& BenchmarkGroupModule::GetBenchmarkGroup() const
	{
		return *_benchmarkGroup;
	}

	BenchmarkGroupModule::BenchmarkGroupModule(const String& filePath)
		: _benchmarkGroup(nullptr, nullptr)
	{
		_modulePtr = LoadModule(filePath);
		auto creator = (BenchmarkGroupCreator)GetFunction(BenchmarkGroupCreatorName, _modulePtr);

		if (creator == nullptr)
		{
			throw ElpidaException("Module '", filePath, "' does not export the required function called '", BenchmarkGroupCreatorName, "'");
		}

		auto destroyer = (BenchmarkGroupDestructor)GetFunction(BenchmarkGroupDestructorName, _modulePtr);

		if (destroyer == nullptr)
		{
			throw ElpidaException("Module '", filePath, "' does not export the required function called '", BenchmarkGroupDestructorName, "'");
		}

		_benchmarkGroup = UniquePtr<BenchmarkGroup, BenchmarkGroupDestructor>(creator(), destroyer);
	}

	BenchmarkGroupModule::BenchmarkGroupModule(BenchmarkGroupModule&& other) noexcept
		: _benchmarkGroup(nullptr, nullptr)
	{
		_benchmarkGroup = std::move(other._benchmarkGroup);
		_modulePtr = other._modulePtr;
		other._modulePtr = nullptr;
	}

	BenchmarkGroupModule& BenchmarkGroupModule::operator=(BenchmarkGroupModule&& other) noexcept
	{
		_benchmarkGroup = std::move(other._benchmarkGroup);
		_modulePtr = other._modulePtr;
		other._modulePtr = nullptr;
		return *this;
	}

	BenchmarkGroupModule::~BenchmarkGroupModule()
	{
		_benchmarkGroup.reset();
		if (_modulePtr != nullptr)
		{
			UnloadModule(_modulePtr);
		}
	}
} // Elpida