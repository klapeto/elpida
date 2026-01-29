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
// Created by klapeto on 21/3/2023.
//

#ifndef ELPIDA_OVERHEADSMODEL_HPP
#define ELPIDA_OVERHEADSMODEL_HPP

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Iterations.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Models/Abstractions/Model.hpp"

#include <chrono>

namespace Elpida::Application
{

	class TimingModel : public Model
	{
	public:
		[[nodiscard]]
		const Duration& GetNowOverhead() const;

		[[nodiscard]]
		const Duration& GetLoopOverhead() const;

		[[nodiscard]]
		Iterations GetIterationsPerSecond() const;

		TimingModel() = default;
		TimingModel(const Duration& nowOverhead, const Duration& loopOverhead, Iterations iterationsPerSecond);
		~TimingModel() override = default;
	private:
		Duration _nowOverhead;
		Duration _loopOverhead;
		Size _iterationsPerSecond;
	};

} // Application

#endif //ELPIDA_OVERHEADSMODEL_HPP
