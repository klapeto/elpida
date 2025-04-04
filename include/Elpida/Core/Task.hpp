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

//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_TASK_HPP_
#define ELPIDA_TASK_HPP_

#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Optional.hpp"
#include "Elpida/Core/TaskInfo.hpp"
#include "Elpida/Core/RawTaskData.hpp"
#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"
#include "Elpida/Core/SharedPtr.hpp"
#include "Elpida/Core/ConcurrencyMode.hpp"

namespace Elpida
{
	class BenchmarkRunContext;

	class Task
	{
	public:
		virtual void Prepare(SharedPtr<AbstractTaskData> inputData) = 0;

		[[nodiscard]]
		virtual Duration Run();

		[[nodiscard]]
		virtual SharedPtr<AbstractTaskData> Finalize() = 0;

		[[nodiscard]]
		TaskInfo GetInfo() const
		{
			auto info = DoGetInfo();
			info._isMeasured = _measured;
			return info;
		}

		[[nodiscard]]
		virtual Size GetProcessedDataSize() const = 0;

		[[nodiscard]]
		UniquePtr<Task> Duplicate() const;

		Task& SetMeasured(bool measured)
		{
			_measured = measured;
			return *this;
		}

		[[nodiscard]]
		virtual ConcurrencyMode GetAllowedConcurrency() const
		{
			return static_cast<ConcurrencyMode>(static_cast<int>(ConcurrencyMode::CopyInput) | static_cast<int>(ConcurrencyMode::ShareInput));
		}

		[[nodiscard]]
		bool ShouldBeMeasured() const
		{
			return _measured;
		}

		void SetBenchmarkRunContext(const BenchmarkRunContext& runContext);

		Task() = default;
		virtual ~Task() = default;
	protected:
		Optional<Ref<const BenchmarkRunContext>> _benchmarkRunContext;
		bool _measured = false;

		[[nodiscard]]
		virtual TaskInfo DoGetInfo() const = 0;

		virtual void DoRun() = 0;

		[[nodiscard]]
		virtual Duration PostProcessDuration(const Duration& duration) const;

		[[nodiscard]]
		virtual UniquePtr<Task> DoDuplicate() const = 0;

		template<typename T, typename ... TArgs>
		SharedPtr<T> CreateData(TArgs&& ... args)
		{
			return std::make_shared<T>(std::forward<TArgs>(args)...);
		}
	};

} // Elpida

#endif //ELPIDA_TASK_HPP_
