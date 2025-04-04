//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 30/9/2024.
//

#ifndef ELPIDA_CLIMESSAGESERVICE_HPP
#define ELPIDA_CLIMESSAGESERVICE_HPP

#include "Core/MessageService.hpp"

namespace Elpida::Application
{

	class CliMessageService : public MessageService
	{
	public:
		void ShowMessage(const std::string& message) override;
		void ShowWarning(const std::string& message) override;
		void ShowError(const std::string& message) override;

		explicit CliMessageService(bool isQuiet);
	private:
		bool _isQuiet;
	};

} // Application
// Elpida

#endif //ELPIDA_CLIMESSAGESERVICE_HPP
