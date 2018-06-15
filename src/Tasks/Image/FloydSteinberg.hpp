/*
 * FloydSteinberg.hpp
 *
 *  Created on: 11 Ιουν 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_IMAGE_FLOYDSTEINBERG_HPP_
#define SRC_TASKS_IMAGE_FLOYDSTEINBERG_HPP_

#include <type_traits>
#include "Task.hpp"
#include "../../Utilities/Image.hpp"

namespace Elpida
{

	template<typename T>
	class FloydSteinberg: public Task
	{
		public:

			static_assert(std::is_floating_point<T>::value, "Float point type is required to use Floyd-Steinberg algorithm");

			void run()
			{
				size_t width = _targetImage.getWidth();
				size_t height = _targetImage.getHeight();
				Pixel<T>* sourceData = _sourceImage.getData();
				Pixel<T>* targetData = _targetImage.getData();
				for (size_t y = 0; y < height; ++y)
				{
					for (size_t x = 0; x < width; ++x)
					{
						size_t index = (y * width) + x;

						T newColorR = sourceData[index].R < _threshold ? 0.0f : 1.0f;
						T newColorG = sourceData[index].G < _threshold ? 0.0f : 1.0f;
						T newColorB = sourceData[index].B < _threshold ? 0.0f : 1.0f;

						T quantizationErrorR = sourceData[index].R - newColorR;
						T quantizationErrorG = sourceData[index].G - newColorG;
						T quantizationErrorB = sourceData[index].B - newColorB;

						targetData[index].R = newColorR;
						targetData[index].G = newColorG;
						targetData[index].B = newColorB;
						targetData[index].A = 1.0f;

						if (x + 1 < width)
						{
							targetData[index + 1].R += (quantizationErrorR * 7.0f) / 16.0f;	// [x + 1, y]
							targetData[index + 1].G += (quantizationErrorG * 7.0f) / 16.0f;
							targetData[index + 1].B += (quantizationErrorB * 7.0f) / 16.0f;
							targetData[index + 1].A = 1.0f;
						}

						if (y + 1 < height)
						{
							if (x - 1 >= 0)
							{
								targetData[index + width - 1].R += (quantizationErrorR * 3.0f) / 16.0f;	// [x - 1, y + 1]
								targetData[index + width - 1].G += (quantizationErrorG * 3.0f) / 16.0f;
								targetData[index + width - 1].B += (quantizationErrorB * 3.0f) / 16.0f;
								targetData[index + width - 1].A = 1.0f;
							}

							targetData[index + width].R += (quantizationErrorR * 5.0f) / 16.0f;	// [x, y + 1]
							targetData[index + width].G += (quantizationErrorG * 5.0f) / 16.0f;
							targetData[index + width].B += (quantizationErrorB * 5.0f) / 16.0f;
							targetData[index + width].A = 1.0f;

							if (x + 1 < width)
							{
								targetData[index + width + 1].R += quantizationErrorR / 16.0f;	// [x + 1, y + 1]
								targetData[index + width + 1].G += quantizationErrorG / 16.0f;
								targetData[index + width + 1].B += quantizationErrorB / 16.0f;
								targetData[index + width + 1].A = 1.0f;
							}
						}
					}
				}
			}

			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const
			{
				return TaskThroughput(
				        TaskThroughput::getValueScale(((double) _sourceImage.getTotalSize()) / metrics.getSeconds()) + "Pixel/s");
			}

			FloydSteinberg(const Image<T>& sourceImage, Image<T>& targetImage, T threshold) :
					Task("Floyd-Steinberg Dithering (threshold: " + std::to_string(threshold) + ")"), _sourceImage(sourceImage), _targetImage(targetImage), _threshold(threshold)
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

#endif /* SRC_TASKS_IMAGE_FLOYDSTEINBERG_HPP_ */