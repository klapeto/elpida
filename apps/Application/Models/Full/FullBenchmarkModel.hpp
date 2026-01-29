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
// Created by klapeto on 22/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKMODEL_HPP
#define ELPIDA_FULLBENCHMARKMODEL_HPP

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"
#include "FullBenchmarkResultModel.hpp"

namespace Elpida::Application
{

	class FullBenchmarkModel : public ListModel<FullBenchmarkResultModel>
	{
	public:
		const std::string& GetCurrentRunningBenchmark() const;
		size_t GetTotalBenchmarks() const;
		bool IsRunning() const;

		Event<bool>& RunningChanged() const;
		Event<const std::string&>& CurrentRunningBenchmarkChanged() const;

		void SetCurrentRunningBenchmark(const std::string& benchmark);
		void SetRunning(bool running);
		void SetTotalBenchmarks(std::size_t totalBenchmarks);

		FullBenchmarkModel();
		~FullBenchmarkModel() override = default;
	private:
		std::string _currentRunningBenchmark;
		std::size_t _totalBenchmarks;
		bool _running;
		mutable Event<bool> _runningChanged;
		mutable Event<const std::string&> _currentRunningBenchmarkChanged;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKMODEL_HPP
