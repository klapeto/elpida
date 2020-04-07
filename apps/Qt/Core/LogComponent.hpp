//
// Created by klapeto on 7/4/20.
//

#ifndef LOGCOMPONENT_HPP
#define LOGCOMPONENT_HPP


#include <sstream>
#include <Elpida/Utilities/Logger.hpp>
#include "Core/Abstractions/VisitableComponent.hpp"

namespace Elpida
{
	class LogsDialog;

	class LogComponent final : VisitableComponent<LogComponent>
	{
	public:
		template<typename ... TArgs>
		void log(Logger::LogType type, TArgs ... args){
			_logger.log(type, args...);
		}

		void showDialog() const;

		LogComponent(ComponentAdapter& adapter);
		~LogComponent();
	private:
		Logger _logger;
		std::stringstream _logAccumulator;
		ComponentAdapter& _adapter;
		LogsDialog* _dialog;
	};
}


#endif //LOGCOMPONENT_HPP
