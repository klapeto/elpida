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
// Created by klapeto on 7/9/2024.
//

#ifndef ELPIDA_SVGRASTERIZATION2TASK_HPP
#define ELPIDA_SVGRASTERIZATION2TASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{

	class SvgRasterization2Task: public MicroTask
	{
	public:

		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		explicit SvgRasterization2Task(std::size_t subSamples, bool multiThread);
		~SvgRasterization2Task() override = default;
	protected:
		void DoRunImpl() override;

		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		SvgCalculatedDocument* _inputDocument;
		SvgBackDrop _rasterizedImage;
		SvgSuperSampler _superSampler;
		bool _multiThread;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATION2TASK_HPP
