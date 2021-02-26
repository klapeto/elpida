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

/*
 * WriteFile.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_COMMONTASKS_WRITEFILE_HPP_
#define ELPIDA_COMMONTASKS_WRITEFILE_HPP_

#include <string>

#include "Elpida/Engine/Task/WorkloadTask.hpp"

namespace Elpida
{
	class TaskMetrics;

	class WriteFile : public WorkloadTask
	{
	public:

		WriteFile(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			std::string filePath,
			size_t iterationsToRun);
		~WriteFile() override = default;
	protected:
		void run() override;
		[[nodiscard]] double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		std::string _outputPath;
	};

} /* namespace Elpida */

#endif /* ELPIDA_COMMONTASKS_WRITEFILE_HPP_ */
