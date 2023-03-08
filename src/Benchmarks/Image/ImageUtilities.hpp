//
// Created by klapeto on 5/3/2023.
//

#ifndef _IMAGEUTILITIES_HPP_
#define _IMAGEUTILITIES_HPP_

#include "Elpida/RawTaskData.hpp"

namespace Elpida
{

	class ImageUtilities final
	{
	 public:
		static void TransformMetadataForChunk(
			std::size_t originalSize,
			std::size_t targetSize);

		static std::size_t GetChunkDivisibleBy(const RawTaskData& taskData);
	};

} // Elpida

#endif //_IMAGEUTILITIES_HPP_
