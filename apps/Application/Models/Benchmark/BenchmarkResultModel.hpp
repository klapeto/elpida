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
// Created by klapeto on 22/8/2023.
//

#ifndef ELPIDA_BENCHMARKRESULTMODEL_HPP
#define ELPIDA_BENCHMARKRESULTMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Elpida/Core/Vector.hpp"
#include "TaskResultModel.hpp"
#include "Elpida/Core/ResultType.hpp"
#include <unordered_map>

namespace Elpida::Application
{

	class BenchmarkModel;

	class BenchmarkResultModel final : public Model
	{
	public:

		double GetResult() const
		{
			return _result;
		}

		const std::string& GetUuid() const
		{
			return _uuid;
		}

		void SetUuid(const std::string& uuid)
		{
			_uuid = uuid;
		}

		const std::string& GetInstanceName() const
		{
			return _instanceName;
		}

		void SetInstanceName(const std::string& instanceName)
		{
			_instanceName = instanceName;
		}

		const BenchmarkModel& GetBenchmark() const
		{
			return *_benchmark;
		}

		const std::unordered_map<std::string, std::string>& GetConfiguration() const
		{
			return _configuration;
		}

		BenchmarkResultModel(
				const BenchmarkModel& benchmark,
				double result,
				std::unordered_map<std::string, std::string>&& configuration)
				: _configuration(std::move(configuration)), _result(result), _benchmark(&benchmark)
		{

		}

		BenchmarkResultModel(const BenchmarkResultModel&) = default;
		BenchmarkResultModel(BenchmarkResultModel&&) noexcept = default;
		BenchmarkResultModel& operator=(const BenchmarkResultModel&) = default;
		BenchmarkResultModel& operator=(BenchmarkResultModel&&) noexcept = default;
		~BenchmarkResultModel() override = default;
	private:
		std::unordered_map<std::string, std::string> _configuration;
		std::string _instanceName;
		std::string _uuid;
		double _result;
		const BenchmarkModel* _benchmark;
	};

} // Application

#endif //ELPIDA_BENCHMARKRESULTMODEL_HPP
