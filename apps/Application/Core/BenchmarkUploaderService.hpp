//
// Created by klapeto on 13/10/2023.
//

#ifndef ELPIDA_BENCHMARKUPLOADERSERVICE_HPP
#define ELPIDA_BENCHMARKUPLOADERSERVICE_HPP


namespace Elpida::Application
{
	class BenchmarkUploaderService final
	{
	public:
		explicit BenchmarkUploaderService();
		~BenchmarkUploaderService() = default;
	private:
		//EventSubscription<const CollectionItem<BenchmarkResultModel>&> _resultAddedSubscription;

		//void UploadResult(const BenchmarkResultModel& resultModel);
	};

} // Elpida
// Application

#endif //ELPIDA_BENCHMARKUPLOADERSERVICE_HPP
