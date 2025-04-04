//
// Created by klapeto on 6/5/2023.
//
#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/AnonymousPipe.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <cerrno>
#include <cstring>
#include <unistd.h>

namespace Elpida
{
	void AnonymousPipe::Open()
	{
		int pipes[2]{ 0, 0 };
		if (pipe(pipes) == -1)
		{
			throw ElpidaException("Failed to create pipe:", strerror(errno));
		}
		_readHandle = pipes[0];
		_writeHandle = pipes[1];
	}

	void AnonymousPipe::CloseRead()
	{
		if (_readHandle.has_value())
		{
			close(std::any_cast<int>(_readHandle));
			_readHandle = {};
		}
	}

	void AnonymousPipe::CloseWrite()
	{
		if (_writeHandle.has_value())
		{
			close(std::any_cast<int>(_writeHandle));
			_writeHandle = {};
		}
	}

	std::size_t AnonymousPipe::Read(char* buffer, std::size_t size) const
	{
		auto bytesRead = read(std::any_cast<int>(_readHandle), buffer, size);
		if (bytesRead <= 0)
		{
			throw ElpidaException("Failed to read from pipe: ", strerror(errno));
		}
		return bytesRead;
	}

	std::size_t AnonymousPipe::Write(char* buffer, std::size_t size) const
	{
		auto bytesWritten = write(std::any_cast<int>(_writeHandle), buffer, size);
		if (bytesWritten <= 0)
		{
			throw ElpidaException("Failed to read from pipe: ", strerror(errno));
		}
		return bytesWritten;
	}

	bool AnonymousPipe::IsOpen() const
	{
		return _readHandle.has_value();
	}

} // Elpida

#endif // defined(ELPIDA_UNIX)
