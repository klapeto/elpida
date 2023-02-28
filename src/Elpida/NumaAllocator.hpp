//
// Created by klapeto on 27/2/2023.
//

#ifndef _NUMAALLOCATOR_HPP_
#define _NUMAALLOCATOR_HPP_

#include <cstdlib>
#include <memory>

namespace Elpida
{
	class NumaAllocator
	{
	 public:
		static void* Allocate(int numaNodeId, std::size_t size);
		static void Deallocate(void* ptr, std::size_t size);
		static int GetProcessorNumaNode(int processorId);
		NumaAllocator() = delete;
	};

	template<typename T>
	class NumaDeleter
	{
	 public:
		void operator()(T* ptr)
		{
			NumaAllocator::Deallocate(ptr, _size);
		}

		explicit NumaDeleter(std::size_t size)
			: _size(size)
		{

		}
	 private:
		std::size_t _size;
	};

	template<typename T>
	class NumaDeleter<T[]>
	{
	 public:
		void operator()(T* ptr)
		{
			NumaAllocator::Deallocate(ptr, _size);
		}

		explicit NumaDeleter(std::size_t size)
			: _size(size)
		{

		}
	 private:
		std::size_t _size;
	};

	class NumaUniquePtr : public std::unique_ptr<char, NumaDeleter<char>>
	{
	 public:
		NumaUniquePtr()
			: std::unique_ptr<char, NumaDeleter<char>>(nullptr, NumaDeleter<char>(0))
		{

		}

		explicit NumaUniquePtr(int numaNodeId, std::size_t size)
			: std::unique_ptr<char, NumaDeleter<char>>((char*)NumaAllocator::Allocate(numaNodeId, size), NumaDeleter<char>(size))
		{

		}
	};

} // Elpida

#endif //_NUMAALLOCATOR_HPP_
