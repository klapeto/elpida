//
// Created by klapeto on 20/4/20.
//

#include "Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp"
#include "Elpida/CommonTasks/WriteFile/WriteFile.hpp"

namespace Elpida
{

	Task* WriteFileSpecification::createNewTask(const TaskConfiguration& configuration,
		const TaskAffinity& affinity) const
	{
		auto filepath = getSettingAndValidate<std::string>(configuration,
			filePathSetting,
			ConfigurationType::FilePath);
		return new WriteFile(*this, std::move(affinity), filepath.getValue(), shouldBeCountedOnResults());
	}

	WriteFileSpecification::WriteFileSpecification(bool shouldBeCountedOnResults, bool canBeDisabled)
		: TaskSpecification("Write from memory to file",
		"Writes data received from other tasks to file on disk",
		"Data to write to disk",
		"Bytes",
		_noOutputString.data(),
		_noOutputString.data(),
		"B",
		{
			new ConfigurationSpecification<std::string>(ConfigurationType::FilePath,std::string(), "Output File Name", "The absolute file path", true)
			},
		true,
		false,
		shouldBeCountedOnResults,
		false,
		canBeDisabled)
	{

	}
}