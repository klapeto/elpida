/*
 * WriteFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Tasks/General/WriteFile.hpp"
#include "Utilities/MemoryFile.hpp"

namespace Elpida
{
	WriteFile::WriteFile(unsigned char* const & data, const size_t& size, const std::string& outputPath) :
			Task("Write File: " + outputPath, false),
			_outputPath(outputPath),
			_data(data),
			_size(size)

	{

	}

	WriteFile::~WriteFile()
	{

	}

	void WriteFile::run()
	{
		MemoryFile(_data, _size).writeToFile(_outputPath);
	}

	TaskThroughput WriteFile::translateToThroutput(const TaskMetrics& metrics) const
	{
		return TaskThroughput(TaskThroughput::getValueScale(((double) _size) / metrics.getSeconds()) + "B/s");
	}

} /* namespace Elpida */

