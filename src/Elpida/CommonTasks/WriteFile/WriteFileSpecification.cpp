//
// Created by klapeto on 20/4/20.
//

#include "Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp"
#include "Elpida/CommonTasks/WriteFile/WriteFile.hpp"

namespace Elpida
{

	Task* WriteFileSpecification::createNewTask(const TaskConfiguration& configuration,
		const TaskAffinity& affinity,
		const std::string& settingsNamespace,
		bool toBeCountedOnResults) const
	{
		auto filepath = getSettingAndValidate<std::string>(configuration,
			settingsNamespace,
			filePathSetting,
			ConfigurationValueBase::Type::FilePath);
		return new WriteFile(*this, std::move(affinity), filepath.getValue(), toBeCountedOnResults);
	}

	WriteFileSpecification::WriteFileSpecification()
		: TaskSpecification("Write from memory to file",
		"Writes data received from other tasks to file on disk",
		"Data to write to disk",
		"Bytes",
		_noOutputString.data(),
		_noOutputString.data(),
		"B")
	{

	}
}