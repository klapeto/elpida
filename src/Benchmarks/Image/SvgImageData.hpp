//
// Created by klapeto on 2/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVGIMAGEDATA_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVGIMAGEDATA_HPP

#include "AllocatorData.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"

#include "nanosvg.h"

namespace Elpida
{

	class SvgImageData final : public SimpleTaskData<NSVGimage*>
	{
	public:
		SvgImageData(NSVGimage* image, const ProcessingUnitNode& targetProcessor, const Allocator& allocator)
			: SimpleTaskData<NSVGimage*>(image, targetProcessor, allocator)
		{
			allocatorData.targetProcessor = &targetProcessor;
			allocatorData.allocator = &allocator;
		}

		SvgImageData(const SvgImageData&) = delete;
		SvgImageData& operator=(const SvgImageData&) = delete;
		SvgImageData(SvgImageData&& other) noexcept
			: SimpleTaskData<NSVGimage*>(other._data, other._targetProcessor, other._allocator)
		{
			other._data = nullptr;

			allocatorData.targetProcessor = &other._targetProcessor.get();
			allocatorData.allocator = &other._allocator.get();
		}

		SvgImageData& operator=(SvgImageData&& other) noexcept
		{
			_targetProcessor = other._targetProcessor;
			_allocator = other._allocator;

			allocatorData.targetProcessor = &_targetProcessor.get();
			allocatorData.allocator = &_allocator.get();
			_data = other._data;
			other._data = nullptr;
			return *this;
		}

		~SvgImageData() override
		{
			if (_data)
			{
				nsvgDelete(_data, &N_Deallocate, &allocatorData);
			}
		}

	private:
		AllocatorData allocatorData;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVGIMAGEDATA_HPP
