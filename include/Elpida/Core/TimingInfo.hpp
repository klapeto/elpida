//
// Created by klapeto on 28/2/2023.
//

#ifndef ELPIDA_OVERHEADSINFO_HPP_
#define ELPIDA_OVERHEADSINFO_HPP_

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Iterations.hpp"

namespace Elpida
{

	enum class TimingStability
	{
		ExtremelyUnstable,
		VeryUnstable,
		Unstable,
		Stable,
		VeryStable,
		ExtremelyStable
	};

	class TimingInfo final
	{
	public:

		[[nodiscard]]
		const Duration& GetMinimumTimeForStableMeasurement() const
		{
			return _minimumTimeForStableMeasurement;
		}

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

		[[nodiscard]]
		TimingStability GetTimingStability() const
		{
			return _timingStability;
		}

		TimingInfo() = default;
		TimingInfo(const TimingInfo&) = default;
		TimingInfo(TimingInfo&&) noexcept = default;
		TimingInfo& operator=(const TimingInfo&) = default;
		TimingInfo& operator=(TimingInfo&&) noexcept = default;
		TimingInfo(const Duration& nowOverhead,
			const Duration& loopOverhead,
			const Duration& virtualCallOverhead,
			const Duration& minimumTimeForStableMeasurement,
			Iterations iterationsPerSecond,
			TimingStability timingStability)
			: _nowOverhead(nowOverhead),
			  _loopOverhead(loopOverhead),
			  _virtualCallOverhead(virtualCallOverhead),
			  _minimumTimeForStableMeasurement(minimumTimeForStableMeasurement),
			  _iterationsPerSecond(iterationsPerSecond),
			  _timingStability(timingStability)
		{
		}
		~TimingInfo() = default;
	private:
		Duration _nowOverhead;
		Duration _loopOverhead;
		Duration _virtualCallOverhead;
		Duration _minimumTimeForStableMeasurement;
		Iterations _iterationsPerSecond;
		TimingStability _timingStability;
	};

} // Elpida

#endif //ELPIDA_OVERHEADSINFO_HPP_
