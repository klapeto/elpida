//
// Created by klapeto on 21/3/2023.
//

#ifndef ELPIDA_OVERHEADSMODEL_HPP
#define ELPIDA_OVERHEADSMODEL_HPP

#include "Models/Abstractions/Model.hpp"

#include <chrono>

namespace Elpida::Application
{

	using Time = std::chrono::duration<double, std::nano>;

	class OverheadsModel : public Model
	{
	public:
		[[nodiscard]]
		const Time& GetNowOverhead() const;

		[[nodiscard]]
		const Time& GetLoopOverhead() const;

		[[nodiscard]]
		const Time& GetVirtualCallOverhead() const;

		OverheadsModel() = default;

		OverheadsModel(const Time& nowOverhead, const Time& loopOverhead, const Time& virtualCallOverhead);

		~OverheadsModel() override = default;

	private:
		Time _nowOverhead;
		Time _loopOverhead;
		Time _virtualCallOverhead;
	};

} // Application

#endif //ELPIDA_OVERHEADSMODEL_HPP
