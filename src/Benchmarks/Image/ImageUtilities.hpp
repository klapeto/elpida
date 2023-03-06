//
// Created by klapeto on 5/3/2023.
//

#ifndef _IMAGEUTILITIES_HPP_
#define _IMAGEUTILITIES_HPP_

#include "Elpida/TaskData.hpp"

namespace Elpida
{

	class ImageUtilities final
	{
	 public:
		static void TransformMetadataForChunk(
			std::size_t originalSize,
			TaskData::Metadata& metadata,
			std::size_t targetSize);

		static std::size_t GetChunkDivisibleBy(const TaskData& taskData);
	};

} // Elpida

#endif //_IMAGEUTILITIES_HPP_
