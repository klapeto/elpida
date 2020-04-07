//
// Created by klapeto on 7/4/20.
//

#ifndef VISITABLECOMPONENT_HPP
#define VISITABLECOMPONENT_HPP

#include "Component.hpp"
#include "ComponentAdapter.hpp"
namespace Elpida
{

	template<typename T>
	class VisitableComponent : public Component
	{
		virtual void visit(ComponentAdapter& adapter) override
		{
			adapter.accept(static_cast<T&>(this));
		}
	};
}


#endif //VISITABLECOMPONENT_HPP
