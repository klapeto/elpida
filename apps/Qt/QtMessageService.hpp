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
