//
// Created by klapeto on 7/4/20.
//

#include "LogComponent.hpp"
#include "Ui/LogsDialog/LogsDialog.hpp"

namespace Elpida
{
	LogComponent::LogComponent(ComponentAdapter& adapter)
		: _adapter(adapter), _dialog(new LogsDialog)
	{
		_logger.setOutput(_logAccumulator);
	}

	LogComponent::~LogComponent()
	{
		delete _dialog;
	}

	void LogComponent::showDialog() const
	{
		_dialog->setLogsText(_logAccumulator.str());
		_dialog->show();
	}
}
