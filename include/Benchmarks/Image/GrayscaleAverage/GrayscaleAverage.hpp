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
 * Grayscale.hpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#ifndef TASKSBATCHES_IMAGE_GRAYSCALEAVERAGE_HPP_
#define TASKSBATCHES_IMAGE_GRAYSCALEAVERAGE_HPP_

#include "Benchmarks/Image/ImageTaskBase.hpp"
#include "Benchmarks/Image/Config.hpp"

namespace Elpida
{
	template<typename T>
	class Image;

	class GrayscaleAverage : public ImageTaskBase
	{
	public:
		void execute() override;

		GrayscaleAverage(const TaskSpecification& specification, const ProcessorNode& processorToRun);
		~GrayscaleAverage() override;
	protected:
		void prepareImpl() override;
		TaskDataDto finalizeAndGetOutputData() override;
		double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	private:
		Image<PixelFloat>* _inputImage;
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_GRAYSCALEAVERAGE_HPP_ */
