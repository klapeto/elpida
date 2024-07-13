//
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_BENCHMARKGROUPLOADER_HPP_
#define ELPIDA_BENCHMARKGROUPLOADER_HPP_

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "Elpida/Core/String.hpp"

namespace Elpida
{

	class BenchmarkGroupModule final
	{
	 public:
		[[nodiscard]]
		const BenchmarkGroup& GetBenchmarkGroup() const;

		explicit BenchmarkGroupModule(const String& filePath);
		BenchmarkGroupModule(BenchmarkGroupModule&&) noexcept;
		BenchmarkGroupModule& operator=(BenchmarkGroupModule&&) noexcept;
		BenchmarkGroupModule(const BenchmarkGroupModule&) = delete;
		BenchmarkGroupModule& operator=(const BenchmarkGroupModule&) = delete;
		~BenchmarkGroupModule();
	 private:
		UniquePtr<BenchmarkGroup, BenchmarkGroupDestructor> _benchmarkGroup;
		void* _modulePtr;

		static void* LoadModule(const String& filePath);
		static void* GetFunction(const char* name, void* module);
		static void UnloadModule(void*);
	};

} // Elpida

#endif //ELPIDA_BENCHMARKGROUPLOADER_HPP_
