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
// Created by klapeto on 19/4/20.
//

#include "QCustomApplication.hpp"

#include <QMessageBox>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"
#include "Models/GlobalConfigurationModel.hpp"

namespace Elpida
{
	QCustomApplication::QCustomApplication(int& argc, char** argv, const GlobalConfigurationModel& configuration, Mediator& mediator, Logger& logger)
		: QApplication(argc, argv), _configuration(configuration), _mediator(mediator), _logger(logger)
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
			if (_configuration.isBlockUI())
			{
				_mediator.waitToContinue();
			}
			return QApplication::notify(obj, event);
		}
		catch (const ElpidaException& ex)
		{
			auto exComponentPresent = !ex.getComponent().empty();
			auto message = Vu::concatenateToString(exComponentPresent ? "Error occurred in: " : "",
				exComponentPresent ? ex.getComponent() : "", exComponentPresent ? "\n" : "",
				ex.what());
			_logger.log(LogType::Error, message, ex);
			_mediator.execute(ShowMessageCommand(message, ShowMessageCommand::Type::Error));
			return false;
		}
		catch (const std::exception& ex)
		{
			_logger.log(LogType::Error,
				Vu::Cs("Critical unhandled error on object: '", qPrintable(obj->objectName()), "'"),
				ex);
			qFatal("Error %s sending event to object (%s)",
				ex.what(),
				qPrintable(obj->objectName()));
			return false;
		}
	}
}
