//
// Created by klapeto on 9/4/20.
//

#ifndef APPS_QT_CORE_QMODELLOGAPPENDER_HPP
#define APPS_QT_CORE_QMODELLOGAPPENDER_HPP

#include <Elpida/Utilities/Logging/LogAppender.hpp>
#include <QStandardItemModel>
#include <QBrush>

namespace Elpida
{
	class QModelLogAppender : public LogAppender
	{
	public:
		QStandardItemModel& getModel()
		{
			return _model;
		}

		void append(LogType logType,
			const Logger::TimeStamp& timeStamp,
			const std::string& message,
			const std::exception* exception) override;
		QModelLogAppender();
		~QModelLogAppender() override = default;
	private:
		QStandardItemModel _model;
		QBrush _errorBrush;
		QBrush _warningBrush;

		QStandardItem* getTypeItem(LogType logType);
	};
}


#endif //APPS_QT_CORE_QMODELLOGAPPENDER_HPP
