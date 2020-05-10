//
// Created by klapeto on 20/4/20.
//

#include "Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp"
#include "Elpida/CommonTasks/ReadFile/ReadFile.hpp"
#include "Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp"

namespace Elpida
{

	Task* ReadFileSpecification::createNewTask(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		auto filePath = getSettingAndValidate<std::string>(configuration,
			Settings::InputFilePath,
			ConfigurationType::Type::FilePath);
		return new ReadFile(*this, affinity, filePath.getValue());
	}

	ReadFileSpecification::ReadFileSpecification()
		: TaskSpecification("Read File to Memory",ResultSpecification("Read Rate", "B", ResultSpecification::Throughput, ResultSpecification::Accumulative))
	{
		withDescription("Reads a file from disk to memory");
		withOutputData(DataSpecification("File contents", "B", "File contents in bytes"));

		withConfiguration(new ConfigurationSpecification<ConfigurationType::FilePath>(ConfigurationType::Type::FilePath,
			"",
			Settings::InputFilePath,
			"The absolute file path",
			true));
	}
}