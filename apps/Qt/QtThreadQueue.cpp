//
// Created by klapeto on 4/5/2023.
//

#include "QtThreadQueue.hpp"
#include <QApplication>
#include <QWindow>

namespace Elpida::Application
{
	bool QtThreadQueue::DefaultProcedure()
	{
		QApplication::sendPostedEvents();
		QApplication::processEvents();

		for (auto* window: QApplication::allWindows())
		{
			if (window->isVisible()) return true;
		}
		return false;
	}

} // Application