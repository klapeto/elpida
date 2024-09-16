//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef ELPIDA_RAYTRACINGTASK_HPP
#define ELPIDA_RAYTRACINGTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Graphics/FrameBuffer.hpp"
#include "Elpida/Graphics/Scene.hpp"
#include "Elpida/Graphics/Camera.hpp"

namespace Elpida
{

	class RayTracingTask : public MicroTask
	{
	public:
		explicit RayTracingTask(size_t size);
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		~RayTracingTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		Graphics::FrameBuffer<double> _frameBuffer;
		Graphics::Scene<double> _scene;
		Graphics::Camera<double> _camera;
		SharedPtr<AbstractTaskData> _inputData;
		std::size_t _size;
	};

}

#endif //ELPIDA_RAYTRACINGTASK_HPP
