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
 * PngLoad.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_IMAGE_PNGDECODING_HPP_
#define SRC_TASKS_IMAGE_PNGDECODING_HPP_

#include "Task.hpp"
#include "Utilities/Image.hpp"

namespace Elpida
{

	class PngDecoding: public Task
	{
		public:

			const Image<unsigned char>& getImage() const
			{
				return _image;
			}

			Image<unsigned char>& getImage()
			{
				return _image;
			}

			void run();
			TaskThroughput translateToThroutput(const TaskMetrics& metrics) const;

			PngDecoding(unsigned char* const& inputData, const size_t& dataSize);
			virtual ~PngDecoding();

			PngDecoding(PngDecoding&&) = default;
			PngDecoding(const PngDecoding&) = default;
			PngDecoding& operator=(PngDecoding&&) = default;
			PngDecoding& operator=(const PngDecoding&) = default;
		private:
			Image<unsigned char> _image;
			unsigned char* const& _inputData;
			const size_t& _dataSize;
	};

} /* namespace Elpida */

#endif /* SRC_TASKS_IMAGE_PNGDECODING_HPP_ */
