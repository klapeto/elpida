//
// Created by klapeto on 6/4/20.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace Elpida
{
	class ComponentAdapter;

	class Component
	{
	public:
		virtual void visit(ComponentAdapter& adapter) = 0;
	};

}

#endif //COMPONENT_HPP
