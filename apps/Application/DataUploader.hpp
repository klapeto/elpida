//
// Created by klapeto on 8/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_DATAUPLOADER_HPP
#define ELPIDA_APPS_APPLICATION_DATAUPLOADER_HPP

#include "Models/Benchmark/BenchmarkResultModel.hpp"

namespace Elpida::Application
{
	class BenchmarkResultModel;
	class OsInfo;
	class CpuInfo;
	class SystemTopology;
	class MemoryInfo;
	class TimingInfo;
	class BenchmarkModel;

	class DataUploader
	{
	public:
		void UploadResult(const BenchmarkModel& benchmark, const BenchmarkResultModel& result);
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_DATAUPLOADER_HPP
