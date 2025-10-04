/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 7/5/2024.
//

#ifndef ELPIDA_SVGDOCUMENTGENERATIONTASK_HPP
#define ELPIDA_SVGDOCUMENTGENERATIONTASK_HPP

#include "Elpida/Core/Task.hpp"

namespace Elpida
{
	class SvgDocumentGenerationTask: public Task
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit SvgDocumentGenerationTask(std::size_t elementCount);
		~SvgDocumentGenerationTask() override = default;
	protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	private:
		std::shared_ptr<AbstractTaskData> _outputData;
		std::size_t _elementCount;
	};

} // Elpida

#endif //ELPIDA_SVGDOCUMENTGENERATIONTASK_HPP
