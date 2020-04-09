//
// Created by klapeto on 9/4/20.
//

#include "QModelLogAppender.hpp"

#include <QStandardItem>
#include <QColor>
#include <string>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>
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

	QModelLogAppender::~QModelLogAppender()
	{

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

	void QModelLogAppender::append(Logger::LogType logType,
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

	QStandardItem* QModelLogAppender::getTypeItem(Logger::LogType logType)
	{
		switch (logType)
		{
		case Logger::LogType::Info:
			return new QStandardItem("Info");
		case Logger::LogType::Error:
		{
			auto item = new QStandardItem("Error");
			item->setForeground(_errorBrush);
			return item;
		}
		case Logger::LogType::Warning:
		{
			auto item = new QStandardItem("Warning");
			item->setForeground(_warningBrush);
			return item;
		}
		}
	}
}