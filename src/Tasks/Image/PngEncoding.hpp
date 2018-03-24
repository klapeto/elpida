/*
 * PngWrite.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_IMAGE_PNGENCODING_HPP_
#define SRC_TASKS_IMAGE_PNGENCODING_HPP_

#include "Task.hpp"
#include "Utilities/Image.hpp"

namespace Elpida
{

	class PngEncoding: public Task
	{
		public:
			unsigned char* const& getEncodedData() const
			{
				return _encodedData;
			}

			const size_t& getEncodedDataSize() const
			{
				return _encodedDataSize;
			}

			void run();
			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const;

			PngEncoding(const Image<unsigned char>& inputImage);
			virtual ~PngEncoding();

			PngEncoding(PngEncoding&&) = default;
			PngEncoding(const PngEncoding&) = default;
			PngEncoding& operator=(PngEncoding&&) = default;
			PngEncoding& operator=(const PngEncoding&) = default;

		private:
			const Image<unsigned char>& _inputImage;
			unsigned char* _encodedData;
			size_t _encodedDataSize;

	};

} /* namespace Elpida */

#endif /* SRC_TASKS_IMAGE_PNGENCODING_HPP_ */
