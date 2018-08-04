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

#ifndef TASKBATCHES_GENERAL_WRITEFILE_HPP_
#define TASKBATCHES_GENERAL_WRITEFILE_HPP_

#include "Elpida/Task.hpp"
#include "Elpida/Types/RawData.hpp"
#include "Elpida/Types/Primitives.hpp"

namespace Elpida
{

	class WriteFile: public Task
	{
		public:

			void run();
			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const;

			WriteFile(const RawDataPtr& data, const Size& size, const String& outputPath);
			virtual ~WriteFile();

			WriteFile(WriteFile&&) = default;
			WriteFile(const WriteFile&) = default;
			WriteFile& operator=(WriteFile&&) = default;
			WriteFile& operator=(const WriteFile&) = default;

		private:
			String _outputPath;
			const RawDataPtr& _data;
			const Size& _size;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_WRITEFILE_HPP_ */
