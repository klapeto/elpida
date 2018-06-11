/*
 * ReadFile.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Tasks/General/ReadFile.hpp"

namespace Elpida
{

	ReadFile::ReadFile(const std::string& filePath) :
			Task("Read File: " + filePath, false),
			_filePath(filePath)
	{

	}

	ReadFile::~ReadFile()
	{

	}

	void ReadFile::run()
	{
		_file.load(_filePath);
	}

	TaskThroughput ReadFile::translateToThroutput(const TaskMetrics& metrics) const
	{
		return TaskThroughput(TaskThroughput::getValueScale(((double)_file.getSize()) / metrics.getSeconds()) + "B/s");
	}

} /* namespace Elpida */
