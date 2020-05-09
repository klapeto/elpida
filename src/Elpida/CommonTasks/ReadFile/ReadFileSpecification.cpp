//
// Created by klapeto on 20/4/20.
//

#include "Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp"
#include "Elpida/CommonTasks/ReadFile/ReadFile.hpp"
#include "Elpida/Engine/Configuration/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/ConfigurationValue.hpp"

namespace Elpida
{

	const char* ReadFileSpecification::filePathSetting = "Input File Name";

	Task* ReadFileSpecification::createNewTaskImpl(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		auto filePath = getSettingAndValidate<std::string>(configuration,
			filePathSetting,
			ConfigurationType::Type::FilePath);
		return new ReadFile(*this, affinity, filePath.getValue(), shouldBeCountedOnResults());
	}

	ReadFileSpecification::ReadFileSpecification(bool shouldBeCountedOnResults,
		bool canBeDisabled,
		const std::string& defaultValue)
		: TaskSpecification("Read File to Memory",ResultSpecification("Read Rate", "B", ResultSpecification::Throughput, ResultSpecification::Accumulative))
	{
		withDescription("Reads a file from disk to memory");
		withOutputData(DataSpecification("File contents", "B", "File contents in bytes"));

		setCanBeDisabled(canBeDisabled);
		setToBeCountedOnResults(shouldBeCountedOnResults);

		auto config = new ConfigurationSpecification<ConfigurationType::FilePath>(ConfigurationType::Type::FilePath,
			defaultValue,
			filePathSetting,
			"The absolute file path",
			true);

		withConfiguration(config);
	}
}