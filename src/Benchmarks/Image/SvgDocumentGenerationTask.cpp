/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 7/5/2024.
//

#include "SvgDocumentGenerationTask.hpp"
#include "Elpida/Core/RawTaskData.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include <cstring>

namespace Elpida
{
	static const char Header[] = R"lit(<svg width="1024" height="1024" viewBox="0 0 1024 1024" version="1.1"><defs><linearGradient id="linearGradient1"><stop style="stop-color:#142bf0;stop-opacity:1;" offset="0" id="stop1"/><stop style="stop-color:#7e86d9;stop-opacity:1;" offset="0.5" id="stop3"/><stop style="stop-color:#709be7;stop-opacity:1;" offset="1" id="stop2"/></linearGradient><radialGradient xlink:href="#linearGradient1" id="radialGradient2" cx="252.40239" cy="497.1463" fx="252.40239" fy="497.1463" r="512" gradientUnits="userSpaceOnUse" gradientTransform="matrix(0.61402064,-0.62817989,1.9737004,2.0192136,-222.39196,417.93027)"/></defs>)lit";

	//static const char Element[] = R"lit(<path style="fill:url(#radialGradient2);fill-rule:evenodd" d="M 772.16627,963.09223 C 335.05463,1226.9562 1019.5565,512.32019 512.04848,517.2374 4.540489,522.15461 702.14923,1223.3994 260.23124,968.05233 -181.68676,712.70523 765.50719,961.853 507.59076,514.66166 249.67432,67.47031 4.8713384,1036.1711 0.06497481,516.9601 -4.7413888,-2.2509497 257.95071,961.53281 507.54228,509.42426 757.13383,57.315706 -185.27791,324.7717 251.83373,60.907765 688.94538,-202.95617 4.4435197,511.6798 511.95153,506.7626 1019.4595,501.84539 321.85077,-199.39945 763.76877,55.94767 1205.6868,311.29477 258.4928,62.146989 516.40925,509.33834 774.32569,956.52969 1019.1287,-12.171139 1023.935,507.0399 1028.7414,1026.2509 766.04929,62.467184 516.45773,514.57575 266.86616,966.6843 1209.2779,699.2283 772.16627,963.09223 Z"/>)lit";
	static const char Element[] = R"lit(<path d="m0 0 292.52 46.194 206.27 138.84 191.76-133.79 333.45-51.247-73.637 281.55-136.68 136.78 151.53 122.49 58.791 483.17-344.7-48.953-203.5-162.51-145.39 157.58-330.41 53.885 48.215-345.15 111.53-182.37-117.03-176.27z" fill="url(#radialGradient2)" fill-rule="evenodd"/>)lit";
	//static const char Element[] = R"lit(<circle style="fill:url(#radialGradient2);fill-rule:evenodd" cx="512" cy="512" r="512"/>)lit";

	static const char Footer[] = R"lit(</svg>)lit";

	SvgDocumentGenerationTask::SvgDocumentGenerationTask(size_t elementCount)
			:_elementCount(elementCount)
	{
		if (_elementCount == 0)
		{
			throw ElpidaException("Attempted to generate a Svg Document with zero elements");
		}
	}

	void SvgDocumentGenerationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());

		// -1 for the null terminator appended
		std::size_t totalSize = sizeof(Header) - 1;
		totalSize += _elementCount * (sizeof(Element) - 1);
		totalSize += sizeof(Footer) - 1;

		_outputData->Allocate(totalSize);
	}

	SharedPtr<AbstractTaskData> SvgDocumentGenerationTask::Finalize()
	{
		return std::move(_outputData);
	}

	Size SvgDocumentGenerationTask::GetProcessedDataSize() const
	{
		return _outputData->GetSize();
	}

	void SvgDocumentGenerationTask::DoRun()
	{
		auto ptr = _outputData->GetData();
		std::memcpy(ptr, Header, sizeof(Header) - 1);
		ptr += sizeof(Header) - 1;

		for (std::size_t i = 0; i < _elementCount; ++i)
		{
			std::memcpy(ptr, Element, sizeof(Element) - 1);
			ptr += sizeof(Element) - 1;
		}

		std::memcpy(ptr, Footer, sizeof(Footer) - 1);
	}

	UniquePtr<Task> SvgDocumentGenerationTask::DoDuplicate() const
	{
		return std::make_unique<SvgDocumentGenerationTask>(_elementCount);
	}

	TaskInfo SvgDocumentGenerationTask::DoGetInfo() const
	{
		return {
				"Svg Document Generation",
				"Generates a document with n amount of elements",
				"B",
				"How many bytes are generated",
				ResultType::Throughput
		};
	}
} // Elpida