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

#ifndef TASKSBATCHES_IMAGE_PNGDECODING_HPP_
#define TASKSBATCHES_IMAGE_PNGDECODING_HPP_

#include <cstddef>

#include "Elpida/Task.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "Elpida/Utilities/Image.hpp"

namespace Elpida
{
	class TaskMetrics;

	class PngDecoding: public Task
	{
		public:

			typedef unsigned char Data;
			typedef Data* DataPtr;

			const Image<Data>& getImage() const
			{
				return _image;
			}

			Image<Data>& getImage()
			{
				return _image;
			}

			void run();
			void calculateResults(const TaskMetrics& metrics);

			PngDecoding(const DataPtr& inputData, const std::size_t& dataSize);
			virtual ~PngDecoding();

			PngDecoding(PngDecoding&&) = default;
			PngDecoding(const PngDecoding&) = default;
			PngDecoding& operator=(PngDecoding&&) = default;
			PngDecoding& operator=(const PngDecoding&) = default;
		private:
			Image<Data> _image;
			TaskRunResult _runResult;
			const DataPtr& _inputData;
			const std::size_t& _dataSize;
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_PNGDECODING_HPP_ */
