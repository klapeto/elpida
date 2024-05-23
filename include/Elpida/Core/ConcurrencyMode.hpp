//
// Created by klapeto on 19/5/2024.
//

#ifndef ELPIDA_CONCURRENCYMODE_HPP
#define ELPIDA_CONCURRENCYMODE_HPP

namespace Elpida
{
	enum class ConcurrencyMode
	{
		None = 0,
		CopyInput = 1,
		ShareInput = 1 << 1,
		ChunkInput = 1 << 2
	};
}

#endif //ELPIDA_CONCURRENCYMODE_HPP
