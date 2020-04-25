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

	Task* ReadFileSpecification::createNewTask(const TaskConfiguration& configuration,
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
		: TaskSpecification("Read File to Memory",
		"Reads a file from disk to memory",
		_noInputString.data(),
		_noInputString.data(),
		"File contents in bytes",
		"Bytes",
		"B",
		{
			new ConfigurationSpecification<ConfigurationType::FilePath>(ConfigurationType::Type::FilePath,
				defaultValue,
				filePathSetting,
				"The absolute file path",
				true)
		},
		false,
		true,
		shouldBeCountedOnResults,
		false,
		canBeDisabled)
	{
	}
}