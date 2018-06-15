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
