/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include <cstddef>
#include <string>

#include "Elpida/Engine/Task/Task.hpp"

namespace Elpida
{
	class TaskMetrics;

	class WriteFile : public Task
	{
	public:

		void execute() override;

		WriteFile(const TaskSpecification& specification,
			const TaskAffinity& affinity,
			std::string filePath,
			bool toBeCountedOnResults = true);
		~WriteFile() override = default;

	protected:
		void prepareImpl() override;
		TaskData finalizeAndGetOutputData() override;
	private:
		std::string _outputPath;
	};

} /* namespace Elpida */

#endif /* ELPIDA_COMMONTASKS_WRITEFILE_HPP_ */
