//
// Created by klapeto on 4/5/2023.
//

#ifndef ELPIDA_QTTHREADQUEUE_HPP
#define ELPIDA_QTTHREADQUEUE_HPP

#include "Core/ThreadQueue.hpp"

namespace Elpida::Application
{

	class QtThreadQueue final : public ThreadQueue
	{
	public:
		QtThreadQueue() = default;
		~QtThreadQueue() override = default;
	protected:
		bool DefaultProcedure() override;
	};

} // Application

#endif //ELPIDA_QTTHREADQUEUE_HPP
