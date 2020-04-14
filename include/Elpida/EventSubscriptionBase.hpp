//
// Created by klapeto on 14/4/20.
//

#ifndef INCLUDE_ELPIDA_EVENTSUBSCRIPTIONBASE_HPP
#define INCLUDE_ELPIDA_EVENTSUBSCRIPTIONBASE_HPP


namespace Elpida
{
	class EventSubscriptionBase
	{

	public:
		virtual void unsubscribe() = 0;
		virtual ~EventSubscriptionBase() = default;
	};
}


#endif //INCLUDE_ELPIDA_EVENTSUBSCRIPTIONBASE_HPP
