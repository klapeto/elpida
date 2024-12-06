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
		: _pipe(pipe)
	{

	}

	void AsyncPipeReader::StartReading()
	{
		_readerThread = std::thread([this]()
		{
		  char buffer[512];
		  std::size_t bytesRead;
		  try
		  {
			  while ((bytesRead = _pipe.Read(buffer, sizeof(buffer))) > 0)
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