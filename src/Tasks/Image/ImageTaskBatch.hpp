/**************************************************************************
*   elpida - CPU benchmark tool
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

#ifndef SRC_TASKS_IMAGE_IMAGETASKBATCH_HPP_
#define SRC_TASKS_IMAGE_IMAGETASKBATCH_HPP_

#include "TaskBatch.hpp"

namespace Elpida
{

	class ImageTaskBatch final: public TaskBatch
	{
		public:
			ImageTaskBatch();
			~ImageTaskBatch();

			ImageTaskBatch(ImageTaskBatch&&) = default;
			ImageTaskBatch(const ImageTaskBatch&) = default;
			ImageTaskBatch& operator=(ImageTaskBatch&&) = default;
			ImageTaskBatch& operator=(const ImageTaskBatch&) = default;
	};

} /* namespace Elpida */

#endif /* SRC_TASKS_IMAGE_IMAGETASKBATCH_HPP_ */
