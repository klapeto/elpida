//
// Created by klapeto on 31/10/2023.
//

#include "SvgParseTask.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/RawTaskData.hpp"

#include "SvgImageData.hpp"
#include "Xml/XmlParser.hpp"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

#include <cstring>
#include <iostream>

namespace Elpida
{

	void SvgParseTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		if (inputData->GetSize() == 0)
		{
			throw ElpidaException("Parse svg must be supplied with data. It got zero size data.");
		}

		_inputData = std::make_unique<RawTaskData>(
			inputData->GetSize() + 1,
			inputData->GetAllocator());
		std::memcpy(_inputData->GetData(), inputData->GetData(), inputData->GetSize());
		//_inputData->GetData()[inputData->GetSize()] = 0;

		_outputData = std::make_unique<SvgImageData>(nullptr,_inputData->GetAllocator());

		_inputData->GetAllocator()->ResetTime();
//		allocatorData.allocator = &_inputData->GetAllocator();
//		allocatorData.targetProcessor = &_inputData->GetTargetProcessor();
	}

	void SvgParseTask::DoRun()
	{
		XmlParser parser(_inputData->GetAllocator());
		_element = parser.Parse(reinterpret_cast<const char*>(_inputData->GetData()), _inputData->GetSize());
		//_image = nsvgParse(reinterpret_cast<char*>(_inputData->GetData()), "px", 96.0f, &N_Allocate, &N_Deallocate, &N_Reallocate, &this->allocatorData);
	}

	UniquePtr<AbstractTaskData> SvgParseTask::Finalize()
	{
//		if (_image == nullptr)
//		{
//			throw ElpidaException("Failed to parse the SVG Image");
//		}
//
//		auto ptr = reinterpret_cast<NSVGimage**>(_outputData->GetData());
//		*ptr = _image;
//		_image = nullptr;
		return std::move(_outputData);
	}

	UniquePtr<Task> SvgParseTask::DoDuplicate() const
	{
		return std::make_unique<SvgParseTask>();
	}

	bool SvgParseTask::CanBeMultiThreaded() const
	{
		return false;
	}

	TaskInfo SvgParseTask::GetInfo() const
	{
		return TaskInfo("SVG Parsing",
			"Parses SVG data to node tree.",
			"B",
			"How many bytes are parsed per second",
			ScoreType::Throughput);
	}

	Size SvgParseTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}
} // Elpida