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
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_QTMESSAGESERVICE_HPP_
#define ELPIDA_QTMESSAGESERVICE_HPP_

#include "Core/MessageService.hpp"
#include "Core/ThreadQueue.hpp"

class QWidget;

namespace Elpida::Application
{

	class QtMessageService final : public MessageService
	{
	 public:
		void ShowMessage(const std::string& message) override;
		void ShowWarning(const std::string& message) override;
		void ShowError(const std::string& message) override;

		QtMessageService();
		~QtMessageService() = default;
	private:
		std::weak_ptr<ThreadQueue> _mainThreadQueue;
	};

} // Application

#endif //ELPIDA_QTMESSAGESERVICE_HPP_
