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
 * FloydSteinberg.hpp
 *
 *  Created on: 11 Ιουν 2018
 *      Author: klapeto
 */

#ifndef TASKSBATCHES_IMAGE_FLOYDSTEINBERG_HPP_
#define TASKSBATCHES_IMAGE_FLOYDSTEINBERG_HPP_

#include <type_traits>

#include "Elpida/Task.hpp"
#include "Elpida/TaskMetrics.hpp"
#include "Elpida/Types/Float.hpp"
#include "Elpida/Utilities/Image.hpp"

namespace Elpida
{

	template<typename T>
	class FloydSteinberg: public Task
	{
		public:

			static_assert(std::is_floating_point<T>::value, "Float point type is required to use Floyd-Steinberg algorithm");

			void run()
			{
				Size width = _targetImage.getWidth();
				Size height = _targetImage.getHeight();
				Pixel<T>* sourceData = _sourceImage.getData();
				Pixel<T>* targetData = _targetImage.getData();
				for (Size y = 0; y < height; ++y)
				{
					for (Size x = 0; x < width; ++x)
					{
						Size index = (y * width) + x;

						T newColorR = sourceData[index].R < _threshold ? 0.0 : 1.0;
						T newColorG = sourceData[index].G < _threshold ? 0.0 : 1.0;
						T newColorB = sourceData[index].B < _threshold ? 0.0 : 1.0;

						T quantizationErrorR = sourceData[index].R - newColorR;
						T quantizationErrorG = sourceData[index].G - newColorG;
						T quantizationErrorB = sourceData[index].B - newColorB;

						targetData[index].R = newColorR;
						targetData[index].G = newColorG;
						targetData[index].B = newColorB;
						targetData[index].A = 1.0;

						if (x + 1 < width)
						{
							targetData[index + 1].R += (quantizationErrorR * 7.0) / 16.0;	// [x + 1, y]
							targetData[index + 1].G += (quantizationErrorG * 7.0) / 16.0;
							targetData[index + 1].B += (quantizationErrorB * 7.0) / 16.0;
							targetData[index + 1].A = 1.0;
						}

						if (y + 1 < height)
						{
							if (x - 1 >= 0)
							{
								targetData[index + width - 1].R += (quantizationErrorR * 3.0) / 16.0;	// [x - 1, y + 1]
								targetData[index + width - 1].G += (quantizationErrorG * 3.0) / 16.0;
								targetData[index + width - 1].B += (quantizationErrorB * 3.0) / 16.0;
								targetData[index + width - 1].A = 1.0;
							}

							targetData[index + width].R += (quantizationErrorR * 5.0) / 16.0;	// [x, y + 1]
							targetData[index + width].G += (quantizationErrorG * 5.0) / 16.0;
							targetData[index + width].B += (quantizationErrorB * 5.0) / 16.0;
							targetData[index + width].A = 1.0;

							if (x + 1 < width)
							{
								targetData[index + width + 1].R += quantizationErrorR / 16.0;	// [x + 1, y + 1]
								targetData[index + width + 1].G += quantizationErrorG / 16.0;
								targetData[index + width + 1].B += quantizationErrorB / 16.0;
								targetData[index + width + 1].A = 1.0;
							}
						}
					}
				}
			}

			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const
			{
				return TaskThroughput(
				        TaskThroughput::getValueScale(((Float64) _sourceImage.getTotalSize()) / metrics.getSeconds()) + "Pixel/s");
			}

			FloydSteinberg(const Image<T>& sourceImage, Image<T>& targetImage, T threshold)
					:
					  Task("Floyd-Steinberg Dithering (threshold: " + std::to_string(threshold) + ")"),
					  _sourceImage(sourceImage),
					  _targetImage(targetImage),
					  _threshold(threshold)
			{

			}

			virtual ~FloydSteinberg()
			{

			}

		private:
			const Image<T>& _sourceImage;
			Image<T>& _targetImage;

			T _threshold;

	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_FLOYDSTEINBERG_HPP_ */