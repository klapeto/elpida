/**************************************************************************
 *   elpida - CPU benchmark tool
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

#ifndef SRC_TASKS_IMAGE_GRAYSCALEAVERAGE_HPP_
#define SRC_TASKS_IMAGE_GRAYSCALEAVERAGE_HPP_

#include "Task.hpp"
#include "Utilities/Image.hpp"
#include "Utilities/ElpidaException.hpp"

namespace Elpida
{
	template<typename T>
	class GrayscaleAverage: public Task
	{

		public:

			void run()
			{
				size_t size = _targetImage.getTotalSize();
				Pixel<T>* sourceData = _sourceImage.getData();
				Pixel<T>* targetData = _targetImage.getData();
				for (size_t i = 0; i < size; ++i)
				{
					T newColor = (sourceData[i].R + sourceData[i].G + sourceData[i].B) / 3.0f;
					targetData[i].R = newColor;
					targetData[i].G = newColor;
					targetData[i].B = newColor;
					targetData[i].A = 1.0f;
				}
			}

			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const
			{
				return TaskThroughput(
				        TaskThroughput::getValueScale(((double) _sourceImage.getTotalSize()) / metrics.getSeconds()) + "Pixel/s");
			}

			void prepare()
			{
				if (!_sourceImage.isCompatibleWith(_targetImage))
				{
					throw ElpidaException("Grayscale", "Images are not the same size!");
				}
			}

			GrayscaleAverage(const Image<T>& sourceImage, Image<T>& targetImage) :
					Task("Convert to Grayscale (Average)"), _sourceImage(sourceImage), _targetImage(targetImage)
			{

			}

			GrayscaleAverage(Image<T> &image) :
					_sourceImage(image), _targetImage(image)
			{

			}

			~GrayscaleAverage()
			{

			}

		private:
			const Image<T> &_sourceImage;
			Image<T> &_targetImage;

	};

} /* namespace Elpida */

#endif /* SRC_TASKS_IMAGE_GRAYSCALEAVERAGE_HPP_ */
