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

#include "Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp"

#include "Elpida/CommonTasks/ReadFile/ReadFile.hpp"
#include "Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp"

namespace Elpida
{

	std::unique_ptr<Task> ReadFileSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		auto& filePath = getSettingAndValidate<ConfigurationType::FilePath>(configuration,
			Settings::InputFilePath,
			ConfigurationType::Type::FilePath);
		return std::make_unique<ReadFile>(*this, processorToRun, serviceProvider, filePath.getValue(), iterationsToRun);
	}

	ReadFileSpecification::ReadFileSpecification()
		: TaskSpecification("Read File to Memory",
		ResultSpecification("Read Rate", "B", ResultType::Throughput, ResultSpecification::Accumulative),
		Uuid)
	{
		withDescription("Reads a file from disk to memory");
		withOutputData(DataSpecification("File contents", "B", "File contents in bytes"));

		withConfiguration(std::make_shared<ConfigurationSpecification<ConfigurationType::FilePath>>(ConfigurationType::Type::FilePath,
			"",
			Settings::InputFilePath,
			"The absolute file path",
			true));
	}
}