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
// Created by klapeto on 20/6/20.
//

#ifndef APPS_QT_CORE_COMMANDS_HTTPRESPONSEEVENT_HPP
#define APPS_QT_CORE_COMMANDS_HTTPRESPONSEEVENT_HPP

#include "Core/Abstractions/TypedCommand.hpp"
#include <string>
#include <utility>
#include <unordered_map>

namespace Elpida
{
	class HttpResponseEvent final : public TypedCommand<HttpResponseEvent>
	{
	public:

		[[nodiscard]]
		int getResponseCode() const
		{
			return _responseCode;
		}

		[[nodiscard]]
		const std::string& getResponse() const
		{
			return _response;
		}

		const std::unordered_map<std::string, std::string>& getResponseHeaders() const
		{
			return _responseHeaders;
		}

		explicit HttpResponseEvent(std::string response,
			int responseCode,
			std::unordered_map<std::string, std::string> responseHeaders)
			: _response(std::move(response)), _responseHeaders(std::move(responseHeaders)), _responseCode(responseCode)
		{
		}
		~HttpResponseEvent() override = default;
	private:
		std::string _response;
		std::unordered_map<std::string, std::string> _responseHeaders;
		int _responseCode;
	};
}


#endif //APPS_QT_CORE_COMMANDS_HTTPRESPONSEEVENT_HPP
