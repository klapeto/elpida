/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 20/4/20.
//

#include "Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp"

#include "Elpida/CommonTasks/WriteFile/WriteFile.hpp"

namespace Elpida
{

	std::unique_ptr<Task> WriteFileSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		auto& filepath = getSettingAndValidate<ConfigurationType::FilePath>(configuration,
			Settings::OutputFilePath,
			ConfigurationType::Type::FilePath);
		return std::make_unique<WriteFile>(*this, processorToRun, serviceProvider, filepath.getValue(), iterationsToRun);
	}

	WriteFileSpecification::WriteFileSpecification()
		: TaskSpecification("Write from memory to file",
		ResultSpecification("Write Rate", "B", ResultType::Throughput, ResultSpecification::Accumulative),
		"F52DD25E-B626-4655-8165-6BC0D81C140F")
	{
		withDescription("Writes data received from other tasks to file on disk");
		withInputData(DataSpecification("Data to write", "B", "The bytes to write to file"));

		withConfiguration(std::make_shared<ConfigurationSpecification<ConfigurationType::FilePath>>(ConfigurationType::Type::FilePath,
			"",
			Settings::OutputFilePath,
			"The absolute file path",
			true));
	}
}