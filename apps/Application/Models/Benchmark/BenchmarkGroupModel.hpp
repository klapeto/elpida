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
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_BENCHMARKGROUPMODEL_HPP_
#define ELPIDA_BENCHMARKGROUPMODEL_HPP_

#include "BenchmarkModel.hpp"

#include <string>

namespace Elpida::Application
{

	class BenchmarkGroupModel : public Model
	{
	 public:
		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::vector<BenchmarkModel>& GetBenchmarks() const;

		BenchmarkGroupModel(std::string name, std::vector<BenchmarkModel>&& benchmarks);
		BenchmarkGroupModel(const BenchmarkGroupModel&) = default;
		BenchmarkGroupModel(BenchmarkGroupModel&&) noexcept = default;
		BenchmarkGroupModel& operator=(const BenchmarkGroupModel&) = default;
		BenchmarkGroupModel& operator=(BenchmarkGroupModel&&) noexcept = default;
		~BenchmarkGroupModel() override = default;
	 private:
		std::string _name;
		std::vector<BenchmarkModel> _benchmarks;
	};

} // Application

#endif //ELPIDA_BENCHMARKGROUPMODEL_HPP_
