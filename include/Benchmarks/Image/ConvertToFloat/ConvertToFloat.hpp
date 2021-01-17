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
 * ConvertToFloat.hpp
 *
 *  Created on: 12 Ιουν 2018
 *      Author: klapeto
 */

#ifndef TASKSBATCHES_IMAGE_CONVERTTOFLOAT_HPP_
#define TASKSBATCHES_IMAGE_CONVERTTOFLOAT_HPP_

#include <cstddef>

#include "Benchmarks/Image/Config.hpp"
#include "Benchmarks/Image/ImageTaskBase.hpp"
#include "Benchmarks/Image/Image.hpp"
#include <Elpida/Engine/Data/RawTaskData.hpp>

namespace Elpida
{
	class ConvertToFloat final : public ImageTaskBase
	{
	public:
		void execute() override;

		ConvertToFloat(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			size_t iterationsToRun);
		~ConvertToFloat() override = default;
	protected:
		void prepareImpl() override;
		std::optional<TaskDataDto> finalizeAndGetOutputData() override;
		[[nodiscard]] double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		std::unique_ptr<RawTaskData> _outputData;
		std::unique_ptr<Image<PixelInt>> _inputImage;
		std::unique_ptr<Image<PixelFloat>> _outputImage;
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_CONVERTTOFLOAT_HPP_ */
