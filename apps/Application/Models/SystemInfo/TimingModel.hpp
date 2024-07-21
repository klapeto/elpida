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
