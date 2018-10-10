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
 * ImageTaskBatch.hpp
 *
 *  Created on: 13 Μαρ 2018
 *      Author: klapeto
 */

#ifndef TASKSBATCHES_IMAGE_IMAGETASKBATCH_HPP_
#define TASKSBATCHES_IMAGE_IMAGETASKBATCH_HPP_

#include "Elpida/TaskBatch.hpp"

namespace Elpida
{

	class ImageTaskBatch final: public TaskBatch
	{
		public:

			const String& getInputFile() const
			{
				return _inputFile;
			}

			void setInputFile(const String& inputFile)
			{
				_inputFile = inputFile;
			}

			const String& getOutputFile() const
			{
				return _outputFile;
			}

			void setOutputFile(const String& outputFile)
			{
				_outputFile = outputFile;
			}

			void reconfigure(const String& inputData);

			ImageTaskBatch();
			~ImageTaskBatch();

			ImageTaskBatch(ImageTaskBatch&&) = default;
			ImageTaskBatch(const ImageTaskBatch&) = default;
			ImageTaskBatch& operator=(ImageTaskBatch&&) = default;
			ImageTaskBatch& operator=(const ImageTaskBatch&) = default;
		private:
			String _inputFile;
			String _outputFile;
			bool _outputEnabled;

			void createTasks();
	};

} /* namespace Elpida */

#endif /* TASKSBATCHES_IMAGE_IMAGETASKBATCH_HPP_ */
