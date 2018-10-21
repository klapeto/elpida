/*
 * Memory.hpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_GENERAL_MEMORY_HPP_
#define TASKBATCHES_GENERAL_MEMORY_HPP_

#include <Elpida/Types/Primitives.hpp>
#include <Elpida/Utilities/NonCopyable.hpp>
#include <mutex>

namespace Elpida
{

	class Memory: public NonCopyable
	{
		public:
			void* getPointer() const
			{
				return _pointer;
			}

			Size getSize() const
			{
				return _size;
			}

			void setSize(Size size)
			{
				_size = size;
			}

			void allocate()
			{
				std::unique_lock<std::mutex> lock(_mutex);
				if (_pointer != nullptr)
				{
					deallocateImpl();
					_pointer = nullptr;
				}
				allocateImpl();
			}
			void deallocate()
			{
				std::unique_lock<std::mutex> lock(_mutex);
				if (_pointer != nullptr)
				{
					deallocateImpl();
					_pointer = nullptr;
				}
			}

			Memory(Size size)
					: _size(size), _pointer(nullptr)
			{

			}

			virtual ~Memory()
			{

			}

		private:
			std::mutex _mutex;
		protected:
			Size _size;
			void* _pointer;

			virtual void allocateImpl() = 0;
			virtual void deallocateImpl() = 0;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_MEMORY_HPP_ */
