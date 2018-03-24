/*
 * MemoryFile.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_UTILITIES_MEMORYFILE_HPP_
#define SRC_UTILITIES_MEMORYFILE_HPP_

#include <string>

namespace Elpida
{

	class MemoryFile
	{
		public:

			unsigned char* const& getData() const
			{
				return _data;
			}

			const size_t& getSize() const
			{
				return _size;
			}

			void load(const std::string& path);
			void writeToFile(const std::string& path) const;

			MemoryFile();
			MemoryFile(size_t size);
			MemoryFile(void* data, size_t size);
			virtual ~MemoryFile();

			MemoryFile(MemoryFile&&) = default;
			MemoryFile(const MemoryFile&) = default;
			MemoryFile& operator=(MemoryFile&&) = default;
			MemoryFile& operator=(const MemoryFile&) = default;
		private:
			unsigned char* _data;
			size_t _size;
			bool _deleteData;
	};

} /* namespace Elpida */

#endif /* SRC_UTILITIES_MEMORYFILE_HPP_ */
