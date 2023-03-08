//
// Created by klapeto on 27/2/2023.
//

#ifndef _NUMAMEMORY_HPP_
#define _NUMAMEMORY_HPP_

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

	class NumaUniquePtr : public std::unique_ptr<unsigned char, NumaDeleter<unsigned char>>
	{
	 public:
		NumaUniquePtr()
			: std::unique_ptr<unsigned char, NumaDeleter<unsigned char>>(nullptr, NumaDeleter<unsigned char>(0))
		{

		}

		explicit NumaUniquePtr(int numaNodeId, std::size_t size)
			: std::unique_ptr<unsigned char, NumaDeleter<unsigned char>>((unsigned char*)NumaAllocator::Allocate(numaNodeId, size), NumaDeleter<unsigned char>(size))
		{

		}
	};

} // Elpida

#endif //_NUMAMEMORY_HPP_
