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

#ifndef TASKSBATCHES_IMAGE_CONVERTTOUINT8_HPP_
#define TASKSBATCHES_IMAGE_CONVERTTOUINT8_HPP_

#include "Elpida/Task.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "Elpida/Types/Float.hpp"
#include "Elpida/Types/Integer.hpp"
#include "Elpida/Utilities/Image.hpp"

namespace Elpida
{
	template<typename T>
	class ConvertToUInt8 final: public Task
	{
		public:

			Image<UInt8>& getImage()
			{
				return _convertedImage;
			}

			void run()
			{
				Size size = _sourceImage.getTotalSize();
				Pixel<UInt8>* converted = _convertedImage.getData();
				Pixel<T>* source = _sourceImage.getData();
				for (Size i = 0; i < size; ++i)
				{
					converted[i].R = source[i].R * 255;
					converted[i].G = source[i].G * 255;
					converted[i].B = source[i].B * 255;
					converted[i].A = source[i].A * 255;
				}
			}

			void calculateResults(const TaskMetrics& metrics)
			{
				_runResult.setOriginalValue(_sourceImage.getTotalSizeInBytes());
				addResult(_runResult);
			}

			void prepare()
			{
				_convertedImage = Image<UInt8>(_sourceImage.getWidth(), _sourceImage.getHeight());
			}

			ConvertToUInt8(const Image<T>& sourceImage)
					: Task("Convert to Uint8"), _sourceImage(sourceImage), _runResult("Data covert rate", "Bytes")
			{

			}

			~ConvertToUInt8()
			{

			}
		private:
			const Image<T>& _sourceImage;
			Image<UInt8> _convertedImage;
			TaskRunResult _runResult;
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_CONVERTTOUINT8_HPP_ */
