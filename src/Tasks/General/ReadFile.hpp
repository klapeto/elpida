/*
 * ReadFile.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_GENERAL_READFILE_HPP_
#define SRC_TASKS_GENERAL_READFILE_HPP_

#include "Task.hpp"
#include "Utilities/MemoryFile.hpp"

namespace Elpida
{

	class ReadFile: public Task
	{
		public:

			const MemoryFile& getFile() const
			{
				return _file;
			}

			void run();
			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const;

			ReadFile(const std::string& filePath);
			virtual ~ReadFile();

			ReadFile(ReadFile&&) = default;
			ReadFile(const ReadFile&) = default;
			ReadFile& operator=(ReadFile&&) = default;
			ReadFile& operator=(const ReadFile&) = default;

		private:
			MemoryFile _file;
			std::string _filePath;
	};

} /* namespace Elpida */

#endif /* SRC_TASKS_GENERAL_READFILE_HPP_ */
