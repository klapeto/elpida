/*
 * WriteFile.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_GENERAL_WRITEFILE_HPP_
#define SRC_TASKS_GENERAL_WRITEFILE_HPP_

#include "Task.hpp"

namespace Elpida
{
	class MemoryFile;

	class WriteFile: public Task
	{
		public:
			void run();
			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const;

			WriteFile(unsigned char* const& data, const size_t& size, const std::string& outputPath);
			virtual ~WriteFile();

			WriteFile(WriteFile&&) = default;
			WriteFile(const WriteFile&) = default;
			WriteFile& operator=(WriteFile&&) = default;
			WriteFile& operator=(const WriteFile&) = default;
		private:
			std::string _outputPath;
			unsigned char* const& _data;
			const size_t& _size;
	};

} /* namespace Elpida */

#endif /* SRC_TASKS_GENERAL_WRITEFILE_HPP_ */
