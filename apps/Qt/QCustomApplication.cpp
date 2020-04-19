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
		// TODO: we should consider: https://doc.qt.io/qt-5/qcoreapplication.html#notify
		// Future direction: This function will not be called for objects that live outside the main thread in Qt 6.
		// Applications that need that functionality should find other solutions for their event inspection needs
		// in the meantime. The change may be extended to the main thread, causing this function to be deprecated.
		//
		// Warning: If you override this function, you must ensure all threads that process events stop doing so before
		// your application object begins destruction. This includes threads started by other libraries that you may be
		// using, but does not apply to Qt's own threads.
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
