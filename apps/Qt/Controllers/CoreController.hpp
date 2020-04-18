//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_CORECONTROLLER_HPP
#define APPS_QT_CONTROLLERS_CORECONTROLLER_HPP

#include <QtWidgets/QApplication>
#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida {
	class CoreController: public CommandHandler
	{
	public:
		void handle(ExitApplicationCommand &command) override;
		void handle(ShowAboutDialogCommand &command) override;
		void handle(ShowMessageCommand &command) override;

		void run();

		CoreController(int& argC, char** argV);
	private:
		QApplication _qApplication;
	};
}




#endif //APPS_QT_CONTROLLERS_CORECONTROLLER_HPP
