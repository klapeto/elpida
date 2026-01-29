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