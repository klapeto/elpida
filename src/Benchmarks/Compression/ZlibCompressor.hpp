//
// Created by klapeto on 31/7/2024.
//

#ifndef ELPIDA_ZLIBCOMPRESSOR_HPP
#define ELPIDA_ZLIBCOMPRESSOR_HPP

#include "Elpida/Core/MemoryStream.hpp"

namespace Elpida
{

	class ZlibCompressor
	{
	public:
		static void Decompress(void* input, std::size_t inputSize, MemoryStream& memoryStream);
		static void Compress(void* input, std::size_t inputSize, MemoryStream& memoryStream);
	};

} // Elpida

#endif //ELPIDA_ZLIBCOMPRESSOR_HPP
