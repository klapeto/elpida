//
// Created by klapeto on 5/4/2024.
//

#ifndef ELPIDA_FUNCTOR_HPP
#define ELPIDA_FUNCTOR_HPP

namespace Elpida
{
	class Functor
	{

	public:
		virtual void operator()() = 0;
		virtual ~Functor() = default;
	};

} // Elpida

#endif //ELPIDA_FUNCTOR_HPP
