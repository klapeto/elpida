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
 * ConvertToFloat.hpp
 *
 *  Created on: 12 Ιουν 2018
 *      Author: klapeto
 */

#ifndef TASKSBATCHES_IMAGE_CONVERTTOFLOAT_HPP_
#define TASKSBATCHES_IMAGE_CONVERTTOFLOAT_HPP_

#include "Elpida/Task.hpp"
#include "Elpida/TaskMetrics.hpp"
#include "Elpida/Types/Float.hpp"
#include "Elpida/Utilities/Image.hpp"

namespace Elpida
{

	template<typename T, typename R = Float32>
	class ConvertToFloat: public Task
	{
		public:

			Image<R>& getImage()
			{
				return _convertedImage;
			}

			void run()
			{
				Size size = _sourceImage.getTotalSize();
				Pixel<T>* sourceData = _sourceImage.getData();
				Pixel<R>* convertedData = _convertedImage.getData();
				for (Size i = 0; i < size; ++i)
				{
					convertedData[i].R = sourceData[i].R / (R) 255.0;
					convertedData[i].G = sourceData[i].G / (R) 255.0;
					convertedData[i].B = sourceData[i].B / (R) 255.0;
					convertedData[i].A = sourceData[i].A / (R) 255.0;
				}
			}

			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const
			{
				return TaskThroughput(
				        TaskThroughput::getValueScale(((Float64) _sourceImage.getTotalSizeInBytes()) / metrics.getSeconds()) + "Bytes/s");
			}

			void prepare()
			{
				_convertedImage = Image<R>(_sourceImage.getWidth(), _sourceImage.getHeight());
			}

			ConvertToFloat(const Image<T>& sourceImage)
					: Task("Convert to Float"), _sourceImage(sourceImage)
			{

			}
			~ConvertToFloat()
			{

			}
		private:
			const Image<T>& _sourceImage;
			Image<R> _convertedImage;
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_CONVERTTOFLOAT_HPP_ */
