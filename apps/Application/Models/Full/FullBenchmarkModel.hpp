//
// Created by klapeto on 22/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKMODEL_HPP
#define ELPIDA_FULLBENCHMARKMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"

namespace Elpida::Application
{

	class FullBenchmarkModel : public Model
	{
	public:
		explicit FullBenchmarkModel(const std::vector<BenchmarkGroupModel>& benchmarkGroups);
		~FullBenchmarkModel() override = default;
	private:
		const BenchmarkModel* _memoryLatency;
		const BenchmarkModel* _memoryReadBandwidth;
		const BenchmarkModel* _pngEncoding;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKMODEL_HPP
