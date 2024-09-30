//
// Created by klapeto on 20/8/2023.
//

#ifndef ELPIDA_ASYNCPIPEREADER_HPP
#define ELPIDA_ASYNCPIPEREADER_HPP

#include "Elpida/Platform/AnonymousPipe.hpp"

#include <thread>
#include <atomic>
#include <sstream>

namespace Elpida
{

	class AnonymousPipe;

	class AsyncPipeReader final
	{
	public:
		void StartReading();
		void StopReading();
		void WaitForDrain();
		std::string GetString();

		explicit AsyncPipeReader(AnonymousPipe& pipe);
		~AsyncPipeReader();
	private:
		std::ostringstream _stringStream;
		std::thread _readerThread;
		AnonymousPipe& _pipe;
		std::atomic<bool> _running;
	};

} // Elpida

#endif //ELPIDA_ASYNCPIPEREADER_HPP
