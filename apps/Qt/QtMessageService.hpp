//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_QTMESSAGESERVICE_HPP_
#define ELPIDA_QTMESSAGESERVICE_HPP_

#include "Core/MessageService.hpp"
namespace Elpida::Application
{

	class QtMessageService final : public MessageService
	{
	 public:
		void ShowMessage(const std::string& message) override;
		void ShowWarning(const std::string& message) override;
		void ShowError(const std::string& message) override;

		QtMessageService() = default;
		~QtMessageService() = default;
	};

} // Application

#endif //ELPIDA_QTMESSAGESERVICE_HPP_
