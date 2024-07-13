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

		[[nodiscard]]
		const Duration& GetVirtualCallOverhead() const
		{
			return _virtualCallOverhead;
		}

		TimingInfo() = default;
		TimingInfo(const TimingInfo&) = default;
		TimingInfo(TimingInfo&&) noexcept = default;
		TimingInfo& operator=(const TimingInfo&) = default;
		TimingInfo& operator=(TimingInfo&&) noexcept = default;

		TimingInfo(const Duration& nowOverhead,
				const Duration& loopOverhead,
				const Duration& virtualCallOverhead,
				Iterations iterationsPerSecond)
				:_nowOverhead(nowOverhead),
				 _loopOverhead(loopOverhead),
				 _virtualCallOverhead(virtualCallOverhead),
				 _iterationsPerSecond(iterationsPerSecond)
		{
		}

		~TimingInfo() = default;
	private:
		Duration _nowOverhead;
		Duration _loopOverhead;
		Duration _virtualCallOverhead;
		Iterations _iterationsPerSecond;
	};

} // Elpida

#endif //ELPIDA_OVERHEADSINFO_HPP_
