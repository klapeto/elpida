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

#ifndef SRC_BENCHMARKS_IMAGE_PNGDECODING_PNGDECODINGSPECIFICATION_HPP
#define SRC_BENCHMARKS_IMAGE_PNGDECODING_PNGDECODINGSPECIFICATION_HPP


#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{
	class PngDecodingSpecification : public TaskSpecification
	{
	public:
		[[nodiscard]]
		std::unique_ptr<Task> createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t iterationsToRun) const override;

		PngDecodingSpecification();
		~PngDecodingSpecification() override = default;
	};
}

#endif //SRC_BENCHMARKS_IMAGE_PNGDECODING_PNGDECODINGSPECIFICATION_HPP
