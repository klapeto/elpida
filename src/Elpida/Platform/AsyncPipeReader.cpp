//
// Created by klapeto on 20/8/2023.
//

#include "Elpida/Platform/AsyncPipeReader.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include <atomic>
#include <string_view>

namespace Elpida
{
	AsyncPipeReader::AsyncPipeReader(AnonymousPipe& pipe)
		: _pipe(pipe), _running(false)
	{

	}

	void AsyncPipeReader::StartReading()
	{
		auto excepted = false;
		if (!_running.compare_exchange_strong(excepted, true)) return;

		_readerThread = std::thread([this]()
		{
		  char buffer[512];
		  std::size_t bytesRead;
		  try
		  {
			  while (_running.load(std::memory_order_acquire) &&
				  (bytesRead = _pipe.Read(buffer, sizeof(buffer))) > 0)
			  {
				  _stringStream << std::string_view(buffer, bytesRead);
			  }
		  }
		  catch (const ElpidaException&)
		  {
			  // Finish reading,
		  }
		});
	}

	void AsyncPipeReader::StopReading()
	{
		auto expected = true;
		if (!_running.compare_exchange_strong(expected, false)) return;
		if (_readerThread.joinable())
		{
			_readerThread.join();
		}
	}

	std::string AsyncPipeReader::GetString()
	{
		return _stringStream.str();
	}

	AsyncPipeReader::~AsyncPipeReader()
	{
		StopReading();
	}

	void AsyncPipeReader::WaitForDrain()
	{
		if (_readerThread.joinable())
		{
			_readerThread.join();
		}
	}
} // Elpida