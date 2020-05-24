//
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Utilities/RawData.hpp"

namespace Elpida
{

	size_t DataAdapter::getAccumulatedSizeOfChunks(const std::vector<const RawData*>& outputChunks)
	{
		size_t outputTotalSize = 0;
		for (auto& data : outputChunks)
		{
			outputTotalSize += data->getSize();
		}
		return outputTotalSize;
	}
}