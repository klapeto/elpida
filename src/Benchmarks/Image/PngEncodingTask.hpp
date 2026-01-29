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
// Created by klapeto on 1/3/2023.
//

#ifndef _PNGENCODINGTASK_HPP_
#define _PNGENCODINGTASK_HPP_

#include <png.h>

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class PngEncodingTask final : public Task
	{
	 public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		PngEncodingTask();
		~PngEncodingTask() override;
	 protected:
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		SharedPtr<AbstractTaskData> _outputData;
		SharedPtr<AbstractTaskData> _inputData;
		png_image _pngImg;
		unsigned int _width;
		unsigned int _height;
	};

} // Elpida

#endif //_PNGENCODINGTASK_HPP_
