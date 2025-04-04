//
// Created by klapeto on 4/5/2023.
//

#ifndef ELPIDA_QTTHREADQUEUE_HPP
#define ELPIDA_QTTHREADQUEUE_HPP

#include "Core/ThreadQueue.hpp"

#include <QObject>
#include <deque>
#include <mutex>

namespace Elpida::Application
{

	class QtThreadQueue final : public QObject, public ThreadQueue
	{
	Q_OBJECT
	public:
		void Enqueue(std::function<void()> func) override;

		int Run() override;

		QtThreadQueue();
		~QtThreadQueue() override = default;
	 signals:
		void ProcedureEnqueued();
	private slots:
		void OnProcedureEnqueued();
	private:
		std::mutex mutex;
		std::deque<std::function<void()>> _queue;
	};

} // Application

#endif //ELPIDA_QTTHREADQUEUE_HPP
