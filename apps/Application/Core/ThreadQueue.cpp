//
// Created by klapeto on 4/5/2023.
//

#include "ThreadQueue.hpp"

namespace Elpida::Application
{
	void ThreadQueue::SetCurrent(std::shared_ptr<ThreadQueue> current)
	{
		_current = std::move(current);
	}

	std::weak_ptr<ThreadQueue> ThreadQueue::Current()
	{
		return _current;
	}
} // Application