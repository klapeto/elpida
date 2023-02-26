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
// Created by klapeto on 9/4/20.
//

#include "QModelLogAppender.hpp"

#include <QStandardItem>
#include <QColor>
#include <string>
#include "Elpida/Common/ElpidaException.hpp"
#include <iomanip>
#include <cmath>
#include <sstream>

namespace Elpida
{

	QModelLogAppender::QModelLogAppender()
		: _errorBrush(QColor(0xcf, 0x53, 0x53)),
		  _warningBrush(QColor(0xce, 0xb5, 0x6b))
	{
		_model.setHorizontalHeaderLabels({ "Timestamp", "Level", "Message", "Exception" });
	}

	static std::string timePointToString(const Logger::TimeStamp& timePoint)
	{
		auto seconds = double(timePoint.time_since_epoch().count()) * Logger::TimeStamp::period::num
			/ Logger::TimeStamp::period::den;
		std::modf(seconds, &seconds);
		std::time_t tt(seconds);
		std::ostringstream oss;
		auto const tm = std::localtime(&tt);
		if (!tm) throw ElpidaException("timepoint to string", strerror(errno));
		oss << std::put_time(tm, "%D %r");
		if (!oss) throw ElpidaException("timepoint to string", "output string stream failed");
		return oss.str();
	}

	void QModelLogAppender::append(LogType logType,
		const Logger::TimeStamp& timeStamp,
		const std::string& message,
		const std::exception* exception)
	{
		QList<QStandardItem*> list;

		list.append(new QStandardItem(QString::fromStdString(timePointToString(timeStamp))));
		list.append(getTypeItem(logType));
		list.append(new QStandardItem(QString::fromStdString(message)));
		list.append(new QStandardItem(QString::fromStdString(exception != nullptr ? exception->what() : "")));

		for (auto item: list)
		{
			item->setEditable(false);
		}

		_model.appendRow(list);
	}

	QStandardItem* QModelLogAppender::getTypeItem(LogType logType)
	{
		switch (logType)
		{
		case LogType::Info:
			return new QStandardItem("Info");
		case LogType::Error:
		{
			auto item = new QStandardItem("Error");
			item->setForeground(_errorBrush);
			return item;
		}
		case LogType::Warning:
		{
			auto item = new QStandardItem("Warning");
			item->setForeground(_warningBrush);
			return item;
		}
		default:
			throw ElpidaException("QModelLogAppender", "Invalid Log type: " + std::to_string((int)logType));
		}
	}
}