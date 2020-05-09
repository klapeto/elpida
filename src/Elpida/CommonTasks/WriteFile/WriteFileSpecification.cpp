//
// Created by klapeto on 20/4/20.
//

#include "Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp"
#include "Elpida/CommonTasks/WriteFile/WriteFile.hpp"

namespace Elpida
{

	const char* WriteFileSpecification::filePathSetting = "Output File Name";

	Task* WriteFileSpecification::createNewTaskImpl(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		auto filepath = getSettingAndValidate<std::string>(configuration,
			filePathSetting,
			ConfigurationType::Type::FilePath);
		return new WriteFile(*this, std::move(affinity), filepath.getValue(), shouldBeCountedOnResults());
	}

	WriteFileSpecification::WriteFileSpecification(bool shouldBeCountedOnResults,
		bool canBeDisabled,
		const std::string& defaultValue)
		: TaskSpecification("Write from memory to file",
		ResultSpecification("Write Rate", "B", ResultSpecification::Throughput, ResultSpecification::Accumulative))
	{
		withDescription("Writes data received from other tasks to file on disk");
		withInputData(DataSpecification("Data to write", "B", "The bytes to write to file"));

		setToBeCountedOnResults(shouldBeCountedOnResults);
		setCanBeDisabled(canBeDisabled);

		withConfiguration(new ConfigurationSpecification<ConfigurationType::FilePath>(ConfigurationType::Type::FilePath,
			defaultValue,
			filePathSetting,
			"The absolute file path",
			true));
	}
}