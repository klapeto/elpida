/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 31/5/20.
//

#ifndef INCLUDE_BENCHMARKS_IMAGE_IMAGETASKBASE_HPP
#define INCLUDE_BENCHMARKS_IMAGE_IMAGETASKBASE_HPP


#include <Elpida/Engine/Task/Task.hpp>

namespace Elpida
{
	class ImageTaskBase : public Task
	{
	public:
		ImageTaskBase(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			size_t iterationsToRun);
		~ImageTaskBase() override = default;
	protected:
		struct InputImageProperties
		{
			size_t width;
			size_t height;
		};

		static InputImageProperties getImageProperties(const TaskDataDto& taskData);
	};
}

#endif //INCLUDE_BENCHMARKS_IMAGE_IMAGETASKBASE_HPP
