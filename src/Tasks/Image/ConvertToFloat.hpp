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

#ifndef SRC_TASKS_IMAGE_CONVERTTOFLOAT_HPP_
#define SRC_TASKS_IMAGE_CONVERTTOFLOAT_HPP_

#include "Task.hpp"
#include "../../Utilities/Image.hpp"

namespace Elpida
{

	template<typename T, typename R = float>
	class ConvertToFloat: public Task
	{
		public:

			Image<R>& getImage()
			{
				return _convertedImage;
			}

			void run()
			{
				size_t size = _sourceImage.getTotalSize();
				Pixel<T>* sourceData = _sourceImage.getData();
				Pixel<float>* convertedData = _convertedImage.getData();
				for (size_t i = 0; i < size; ++i)
				{
					convertedData[i].R = sourceData[i].R / 255.0f;
					convertedData[i].G = sourceData[i].G / 255.0f;
					convertedData[i].B = sourceData[i].B / 255.0f;
					convertedData[i].A = sourceData[i].A / 255.0f;
				}
			}

			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const
			{
				return TaskThroughput(
				        TaskThroughput::getValueScale(((double) _sourceImage.getTotalSizeInBytes()) / metrics.getSeconds()) + "Bytes/s");
			}

			void prepare() {
				_convertedImage = Image<R>(_sourceImage.getWidth(), _sourceImage.getHeight());
			}

			ConvertToFloat(const Image<T>& sourceImage) :
					Task("Convert to Float"),
					_sourceImage(sourceImage)
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

#endif /* SRC_TASKS_IMAGE_CONVERTTOFLOAT_HPP_ */
