/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
