/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ELPIDAMEDIATOR_HPP
#define APPS_QT_CORE_ELPIDAMEDIATOR_HPP

#include "Core/Abstractions/Mediator.hpp"
#include "Core/Abstractions/CommandHandler.hpp"
#include <vector>
#include <functional>

namespace Elpida
{

	class ElpidaMediator final : public Mediator
	{
	public:
		void execute(Command& command) override;
		void execute(Command&& command) override;

		void registerCommandHandler(CommandHandler& handler);

		ElpidaMediator() = default;
		~ElpidaMediator() override = default;
	private:
		std::vector<std::reference_wrapper<CommandHandler>> _handlers;
	};
}

#endif //APPS_QT_CORE_ELPIDAMEDIATOR_HPP
