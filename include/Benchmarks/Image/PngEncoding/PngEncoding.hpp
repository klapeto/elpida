/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include <Benchmarks/Image/Config.hpp>
#include "Benchmarks/Image/ImageTaskBase.hpp"

namespace Elpida
{
	template<typename T>
	class Image;

	class PngEncoding : public ImageTaskBase
	{
	public:

		void execute() override;
		PngEncoding(const TaskSpecification& specification, const ProcessorNode& processorToRun);
		~PngEncoding() override;
	protected:
		void prepareImpl() override;
		TaskDataDto finalizeAndGetOutputData() override;
		double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		RawData* _outputData;
		Image<PixelInt>* _inputImage;
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_PNGENCODING_HPP_ */
