//
// Created by klapeto on 20/4/20.
//

#include "Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp"
#include "Elpida/CommonTasks/ReadFile/ReadFile.hpp"
#include "Elpida/Engine/Configuration/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/ConfigurationValue.hpp"

namespace Elpida
{

	Task* ReadFileSpecification::createNewTask(const TaskConfiguration& configuration,
		const TaskAffinity& affinity,
		const std::string& settingsNamespace,
		bool toBeCountedOnResults) const
	{
		auto filePath = getSettingAndValidate<std::string>(configuration,
			settingsNamespace,
			filePathSetting,
			ConfigurationValueBase::Type::FilePath);
		return new ReadFile(*this, affinity, filePath.getValue(), toBeCountedOnResults);
	}

	ReadFileSpecification::ReadFileSpecification()
		: TaskSpecification("Read File to Memory",
		"Reads a file from disk to memory",
		_noInputString.data(),
		_noInputString.data(),
		"File contents in bytes",
		"Bytes",
		"B")
	{

	}
}