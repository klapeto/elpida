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

} // Elpida