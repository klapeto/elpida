//
// Created by klapeto on 7/4/20.
//

#ifndef COMPONENTADAPTER_HPP
#define COMPONENTADAPTER_HPP


namespace Elpida
{

	class Component;
	class ApplicationEvent;

	class ComponentAdapter
	{
	public:
		virtual void accept(Component& component, const ApplicationEvent& event) = 0;
	};
}


#endif //COMPONENTADAPTER_HPP
