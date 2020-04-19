//
// Created by klapeto on 19/4/20.
//


#include "QCustomApplication.hpp"

#include <QMessageBox>
#include <iostream>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>

namespace Elpida
{
	QCustomApplication::QCustomApplication(int& argc, char** argv)
		: QApplication(argc, argv)
	{

	}

	bool QCustomApplication::notify(QObject* obj, QEvent* event)
	{
		try
		{
			return QApplication::notify(obj, event);
		}
		catch (const ElpidaException& ex)
		{
			auto exComponentPresent = !ex.getComponent().empty();
			QMessageBox::critical(QApplication::activeWindow(),
				QString::fromStdString("Error"),
				QString::fromStdString(Vu::concatenateToString(exComponentPresent ? "Error occurred in: " : "",
					exComponentPresent ? ex.getComponent() : "", exComponentPresent ? "\n" : "",
					ex.what())));
			return false;
		}
		catch (const std::exception& ex)
		{
			qFatal("Error %s sending event to object (%s)",
				ex.what(),
				qPrintable(obj->objectName()));
			return false;
		}

	}

}
