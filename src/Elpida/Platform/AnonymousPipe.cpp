//
// Created by klapeto on 6/5/2023.
//

#include "Elpida/Platform/AnonymousPipe.hpp"

namespace Elpida
{
	AnonymousPipe::~AnonymousPipe()
	{
		Close();
	}

	AnonymousPipe& AnonymousPipe::operator=(AnonymousPipe&& other) noexcept
	{
		Close();
		_readHandle = std::move(other._readHandle);
		_writeHandle = std::move(other._writeHandle);
		return *this;
	}

	void AnonymousPipe::Close()
	{
		CloseRead();
		CloseWrite();
	}

} // Elpida