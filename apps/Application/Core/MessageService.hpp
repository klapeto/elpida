//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_MESSAGESERVICE_HPP_
#define ELPIDA_MESSAGESERVICE_HPP_

#include <string>

namespace Elpida::Application
{

	class MessageService
	{
	 public:
		virtual void ShowMessage(const std::string& message) = 0;
		virtual void ShowWarning(const std::string& message) = 0;
		virtual void ShowError(const std::string& message) = 0;
	};

} // Application

#endif //ELPIDA_MESSAGESERVICE_HPP_
