//
// Created by klapeto on 21/3/2023.
//

#ifndef ELPIDA_OVERHEADSMODEL_HPP
#define ELPIDA_OVERHEADSMODEL_HPP

#include "Elpida/Core/Duration.hpp"
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
		const Duration& GetVirtualCallOverhead() const;

		[[nodiscard]]
		const Duration& GetStableTime() const;

		[[nodiscard]]
		bool IsSystemStable() const;

		TimingModel() = default;
		TimingModel(const Duration& nowOverhead, const Duration& loopOverhead, const Duration& virtualCallOverhead, const Duration stableTime, bool systemStable);
		~TimingModel() override = default;
	private:
		Duration _nowOverhead;
		Duration _loopOverhead;
		Duration _virtualCallOverhead;
		Duration _stableTime;
		bool _systemStable;
	};

} // Application

#endif //ELPIDA_OVERHEADSMODEL_HPP
