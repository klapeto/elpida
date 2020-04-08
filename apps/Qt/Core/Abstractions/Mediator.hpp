//
// Created by klapeto on 8/4/20.
//

#ifndef MEDIATOR_HPP
#define MEDIATOR_HPP

namespace Elpida
{
	class Command;
	class Mediator
	{
	public:
		virtual void execute(const Command& command) = 0;

		virtual ~Mediator()
		{

		}
	};
}

#endif //MEDIATOR_HPP
