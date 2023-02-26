project(Elpida-Engine VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida engine"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-engine
        BenchmarksContainerPlugin.cpp
        ServiceProvider.cpp
        SharedStructuresProvider.cpp

        Benchmark/Benchmark.cpp
        Benchmark/BenchmarkTaskInstance.cpp

        Calculators/BenchmarkScoreCalculator.cpp
        Calculators/Benchmark/AccumulativeScoreCalculator.cpp
        Calculators/Benchmark/AverageScoreCalculator.cpp
        Calculators/Benchmark/AverageThroughputScoreCalculator.cpp
        Calculators/TaskResultCalculator.cpp
        Calculators/Task/ExclusiveHarmonicMeanTaskResultCalculator.cpp
        Calculators/Task/MinimumTaskResultCalculator.cpp

        Configuration/ConfigurationInstance.cpp
        Configuration/Concrete/BenchmarkConfiguration.cpp
        Configuration/Concrete/ConfigurationValue.cpp
        Configuration/Concrete/ConfigurationValueBase.cpp
        Configuration/Concrete/TaskConfiguration.cpp
        Configuration/Specification/ConfigurationSpecification.cpp
        Configuration/Specification/ConfigurationSpecificationBase.cpp

        Data/DataAdapter.cpp
        Data/DataSpecification.cpp
        Data/DataTransformer.cpp
        Data/RawTaskData.cpp

        Result/BenchmarkResult.cpp
        Result/BenchmarkScore.cpp
        Result/BenchmarkScoreSpecification.cpp
        Result/ProcessedTaskResult.cpp
        Result/ResultSpecification.cpp
        Result/TaskMetrics.cpp
        Result/TaskResult.cpp

        Runner/BenchmarkRunner.cpp
        Runner/BenchmarkRunRequest.cpp
        Runner/DefaultTaskRunner.cpp
        Runner/MicroTaskRunner.cpp

        Task/ExecutionParameters.cpp
        Task/MicroTask.cpp
        Task/MultiThreadTask.cpp
        Task/Task.cpp
        Task/TaskAffinity.cpp
        Task/TaskDataDto.cpp
        Task/TaskBuilder.cpp
        Task/TaskSpecification.cpp
        Task/TaskThread.cpp
        Task/WorkloadTask.cpp
        )

target_link_libraries(elpida-engine elpida-common elpida-utilities elpida-systemInfo)
target_include_directories(elpida-engine PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(elpida-engine PUBLIC ${ELPIDA_BINARY_INCLUDE_DIR})