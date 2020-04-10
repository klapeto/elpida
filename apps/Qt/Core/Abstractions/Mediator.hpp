//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_MEDIATOR_HPP
#define APPS_QT_CORE_ABSTRACTIONS_MEDIATOR_HPP

namespace Elpida
{
	class Command;
	class Mediator
	{
	public:
		virtual void execute(const Command& command) = 0;

		virtual ~Mediator() = default;
	};
}

#endif //APPS_QT_CORE_ABSTRACTIONS_MEDIATOR_HPP
