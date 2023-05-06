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
	AnonymousPipe::AnonymousPipe()
	{
		int pipes[2]{ 0, 0 };
		if (pipe(pipes) == -1)
		{
			throw ElpidaException("Failed to create pipe:", strerror(errno));
		}
		_readHandle = pipes[0];
		_writeHandle = pipes[1];
	}

	void AnonymousPipe::Close()
	{
		if (_readHandle.has_value())
		{
			close(GetReadHandle<int>());
		}

		if (_writeHandle.has_value())
		{
			close(GetWriteHandle<int>());
		}
	}

} // Elpida

#endif // defined(ELPIDA_UNIX)
