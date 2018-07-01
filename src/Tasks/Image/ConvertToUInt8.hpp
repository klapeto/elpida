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
 * ConvertToUInt8.hpp
 *
 *  Created on: 12 Ιουν 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_IMAGE_CONVERTTOUINT8_HPP_
#define SRC_TASKS_IMAGE_CONVERTTOUINT8_HPP_

#include "Task.hpp"
#include "../../Utilities/Image.hpp"

namespace Elpida
{
	template<typename T>
	class ConvertToUInt8: public Task
	{
		public:

			Image<uint8_t>& getImage()
			{
				return _convertedImage;
			}

			void run()
			{
				size_t size = _sourceImage.getTotalSize();
				Pixel<uint8_t>* converted = _convertedImage.getData();
				Pixel<T>* source = _sourceImage.getData();
				for (size_t i = 0; i < size; ++i)
				{
					converted[i].R = source[i].R * 255;
					converted[i].G = source[i].G * 255;
					converted[i].B = source[i].B * 255;
					converted[i].A = source[i].A * 255;
				}
			}

			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const
			{
				return TaskThroughput(
				        TaskThroughput::getValueScale(((double) _sourceImage.getTotalSizeInBytes()) / metrics.getSeconds()) + "Bytes/s");
			}

			void prepare() {
				_convertedImage = Image<uint8_t>(_sourceImage.getWidth(), _sourceImage.getHeight());
			}

			ConvertToUInt8(const Image<T>& sourceImage) :
					Task("Convert to Uint8"), _sourceImage(sourceImage)
			{

			}
			~ConvertToUInt8()
			{

			}
		private:
			const Image<T>& _sourceImage;
			Image<uint8_t> _convertedImage;
	};

} /* namespace Elpida */

#endif /* SRC_TASKS_IMAGE_CONVERTTOUINT8_HPP_ */
