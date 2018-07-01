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
