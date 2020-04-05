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

#include <cstddef>

#include "Elpida/Exceptions/ElpidaException.hpp"
#include "Elpida/Task.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "Elpida/Utilities/Imaging/Image.hpp"

namespace Elpida
{
	class TaskMetrics;

	template<typename T>
	class GrayscaleAverage : public Task
	{

	public:

		void run()
		{
			std::size_t size = _targetImage.getTotalSize();
			Pixel<T>* sourceData = _sourceImage.getData();
			Pixel<T>* targetData = _targetImage.getData();
			for (std::size_t i = 0; i < size; ++i)
			{
				T newColor = (sourceData[i].R + sourceData[i].G + sourceData[i].B) / 3.0;
				targetData[i].R = newColor;
				targetData[i].G = newColor;
				targetData[i].B = newColor;
				targetData[i].A = 1.0;
			}
		}

		void calculateResults(const TaskMetrics& metrics)
		{
			_runResult.setOriginalValue(_sourceImage.getTotalSize());
			addResult(_runResult);
		}

		void prepare()
		{
			if (!_sourceImage.isCompatibleWith(_targetImage))
			{
				throw ElpidaException("Grayscale", "Images are not the same size!");
			}
		}

		GrayscaleAverage(const Image<T>& sourceImage, Image<T>& targetImage)
			: Task("Convert to Grayscale (Average)"), _sourceImage(sourceImage), _targetImage(targetImage),
			  _runResult("Pixel process rate", "Pixels")
		{

		}

		GrayscaleAverage(Image<T>& image)
			: _sourceImage(image), _targetImage(image), _runResult("Pixel process rate", "Pixels")
		{

		}

		~GrayscaleAverage()
		{

		}

	private:
		const Image<T>& _sourceImage;
		Image<T>& _targetImage;
		TaskRunResult _runResult;

	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_GRAYSCALEAVERAGE_HPP_ */
