/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * PngWrite.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef TASKSBATCHES_IMAGE_PNGENCODING_HPP_
#define TASKSBATCHES_IMAGE_PNGENCODING_HPP_

#include "Benchmarks/Image/Config.hpp"
#include "Benchmarks/Image/ImageTaskBase.hpp"
#include "Benchmarks/Image/Image.hpp"

#include <Elpida/Engine/Data/RawTaskData.hpp>

namespace Elpida
{
	template<typename T>
	class Image;

	class PngEncoding : public ImageTaskBase
	{
	public:
		PngEncoding(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t iterationsToRun);
		~PngEncoding() override = default;
	protected:
		void run() override;
		void prepareImpl() override;
		std::optional<TaskDataDto> finalizeAndGetOutputData() override;

		[[nodiscard]]
		double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		std::unique_ptr<RawTaskData> _outputData;
		std::unique_ptr<Image<PixelInt>> _inputImage;
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_PNGENCODING_HPP_ */
