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
