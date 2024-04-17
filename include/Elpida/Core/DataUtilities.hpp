//
// Created by klapeto on 8/3/2023.
//

#ifndef ELPIDA_DATAUTILITIES_HPP_
#define ELPIDA_DATAUTILITIES_HPP_

#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/SharedPtr.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

#include <cmath>
#include <cstring>
#include <algorithm>

namespace Elpida
{

	class DataUtilities
	{
		static Size GetAccumulatedSizeOfChunks(const Vector<Ref<const AbstractTaskData>>& outputChunks)
		{
			Size outputTotalSize = 0;
			for (const auto& data: outputChunks)
			{
				outputTotalSize += data.get().GetSize();
			}
			return outputTotalSize;
		}

	 public:
		template<typename T, typename TReturn = T, typename TConstructor>
		static Vector<UniquePtr<TReturn>> SplitChunksToChunks(const Vector<Ref<const AbstractTaskData>>& input,
			const Vector<SharedPtr<Allocator>>& targetAllocators,
			Size chunkDivisibleBy, TConstructor constructor){
			auto targetChunksCount = targetAllocators.size();
			auto& outputChunks = input;

			auto outputTotalSize = GetAccumulatedSizeOfChunks(input);

			Size targetChunkSize = std::ceil((float)outputTotalSize / (float)targetChunksCount);

			while (targetChunkSize % chunkDivisibleBy != 0)
			{
				targetChunkSize++;
			}

			Vector<UniquePtr<TReturn>> targetChunksVec;
			targetChunksVec.reserve(targetChunksCount);

			Size currentChunkIndex = 0;
			const T* currentChunk = nullptr;
			Size currentChunkOffset = 0;
			Size currentChunkSize = 0;
			Size totalDataCopied = 0;
			for (const auto& outChunk: outputChunks)
			{
				Size oChunkSize = outChunk.get().GetSize();
				Size oChunkBytesCopied = 0;

				// while we haven't done copying this output chunk
				while (oChunkBytesCopied < oChunkSize)
				{
					if (currentChunk == nullptr)
					{
						currentChunkSize = std::min(targetChunkSize, outputTotalSize - totalDataCopied);

						// create new chunk with the defined target size
						auto chunk = constructor(targetAllocators[currentChunkIndex], currentChunkSize);
						//chunk->Allocate(currentChunkSize);
						targetChunksVec.push_back(std::move(chunk));

						currentChunk = static_cast<const T*>(targetChunksVec.back().get());
					}

					size_t actualReadSize;
					if (oChunkSize - oChunkBytesCopied > currentChunkSize - currentChunkOffset)
					{
						// Does not fit to current target chunk. This means we need to read UP TO the rest of the target
						// chunk capacity
						actualReadSize = currentChunkSize - currentChunkOffset;
					}
					else
					{
						// Fits to current target chunk. This means that we need to read the remaining bytes of the output
						// chunk
						actualReadSize = oChunkSize - oChunkBytesCopied;
					}

					// copy the data
					std::memcpy(currentChunk->GetData() + currentChunkOffset,
						outChunk.get().GetData() + oChunkBytesCopied,
						actualReadSize);

					// add offsets of the actual copied data
					oChunkBytesCopied += actualReadSize;
					currentChunkOffset += actualReadSize;

					totalDataCopied += actualReadSize;

					if (currentChunkOffset >= currentChunkSize)
					{
						// trigger a chunk creation on next iteration
						currentChunk = nullptr;
						currentChunkOffset = 0;
						currentChunkIndex++;
					}
				}
			}
			return targetChunksVec;
		}
	};

} // Elpida

#endif //ELPIDA_DATAUTILITIES_HPP_
