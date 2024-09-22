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

#include "RayTracingTask.hpp"
#include "ImageTaskData.hpp"
#include "SceneCreator.hpp"

namespace Elpida
{

	using namespace Graphics;

	void RayTracingTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_frameBuffer = FrameBuffer<double>(_size, _size);
		_scene = SceneCreator::GenerateScene<double>();
		_camera = Camera<double>(
				{ -40.0, 40.0, 40.0 },
				{ 0.0, 0.0, 0.0 },
				{ 0.0, 1.0, 0.0 });
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> RayTracingTask::Finalize()
	{
		auto ptr = std::make_unique<ImageTaskData>(_inputData->GetAllocator(), _size, _size, 4, 4);

		auto totalSize = _size * _size;
		ptr->Allocate(totalSize * 4 * 4);
		auto data = reinterpret_cast<float*>(ptr->GetData());
		for (std::size_t i = 0; i < totalSize; ++i)
		{
			auto colorIndex = i * 4;
			auto& color = _frameBuffer.Get(i);
			data[colorIndex] = color.R();
			data[colorIndex + 1] = color.G();
			data[colorIndex + 2] = color.B();
			data[colorIndex + 3] = color.A();
		}

		return ptr;
	}

	Size RayTracingTask::GetProcessedDataSize() const
	{
		return _size * _size;
	}

	void RayTracingTask::DoRunImpl()
	{
		Exec([&]()
		{
			_camera.Render(_scene, _frameBuffer);
		});
	}

	Size RayTracingTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> RayTracingTask::DoDuplicate() const
	{
		return std::make_unique<RayTracingTask>(_size);
	}

	TaskInfo RayTracingTask::DoGetInfo() const
	{
		return {
				"3D Ray Tracing",
				"Ray traces a 3D scene to rasterized image",
				"Rays",
				"The ray tracing rate",
				ResultType::Throughput
		};
	}

	RayTracingTask::RayTracingTask(size_t size)
			:_size(size)
	{
	}

}