//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_MAINVIEWCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_MAINVIEWCONTROLLER_HPP


#include <QtWidgets/QApplication>
#include "Ui/MainWindow/MainWindow.hpp"

class QWidget;

namespace Elpida
{
	class MainViewController
	{
		void addTabView(QWidget& widget, const std::string& name);

	private:
		QApplication _qApplication;
		MainWindow _mainWindow;
	};
}


#endif //APPS_QT_CONTROLLERS_MAINVIEWCONTROLLER_HPP
