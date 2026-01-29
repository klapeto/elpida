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
// Created by klapeto on 28/2/2023.
//

#ifndef ELPIDA_OVERHEADSINFO_HPP_
#define ELPIDA_OVERHEADSINFO_HPP_

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Iterations.hpp"
#include <cstddef>

namespace Elpida
{
	class TimingInfo final
	{
	public:

		[[nodiscard]]
		Iterations GetIterationsPerSecond() const
		{
			return _iterationsPerSecond;
		}

		[[nodiscard]]
		const Duration& GetNowOverhead() const
		{
			return _nowOverhead;
		}

		[[nodiscard]]
		const Duration& GetLoopOverhead() const
		{
			return _loopOverhead;
		}

		TimingInfo() = default;
		TimingInfo(const TimingInfo&) = default;
		TimingInfo(TimingInfo&&) noexcept = default;
		TimingInfo& operator=(const TimingInfo&) = default;
		TimingInfo& operator=(TimingInfo&&) noexcept = default;

		TimingInfo(const Duration& nowOverhead,
				const Duration& loopOverhead,
				Iterations iterationsPerSecond)
				:_nowOverhead(nowOverhead),
				 _loopOverhead(loopOverhead),
				 _iterationsPerSecond(iterationsPerSecond)
		{
		}

		~TimingInfo() = default;
	private:
		Duration _nowOverhead;
		Duration _loopOverhead;
		Iterations _iterationsPerSecond;
	};

} // Elpida

#endif //ELPIDA_OVERHEADSINFO_HPP_
