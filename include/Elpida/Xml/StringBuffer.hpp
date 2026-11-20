/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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

////////////////////////////////////////////////////////////////////////////////

//
// Created by klapeto on 20/11/26.
//

#ifndef ELPIDA_STRINGBUFFER_HPP
#define ELPIDA_STRINGBUFFER_HPP

#include <cstddef>
#include <string>

namespace Elpida
{
	template <std::size_t BUFFER_SIZE = 256, typename T = char>
	class StringBuffer
	{
	public:
		std::basic_string<T>& GetString()
		{
			Flush();
			return _str;
		}

		void Reset()
		{
			_str.clear();
			_size = 0;
		}

		void Append(const std::basic_string<T>& str)
		{
			for (std::size_t i = 0; i < str.size(); ++i)
			{
				CheckAndFlush();
				Append(str[i]);
			}
		}

		void Append(T c)
		{
			CheckAndFlush();
			_buffer[_size++] = c;
		}

		StringBuffer()
			: _size(0)
		{
		}

		StringBuffer(const StringBuffer&) = delete;
		StringBuffer& operator=(const StringBuffer&) = delete;
		StringBuffer(StringBuffer&&) = delete;
		StringBuffer& operator=(StringBuffer&&) = delete;
		~StringBuffer() = default;

	private:
		T _buffer[BUFFER_SIZE];
		std::basic_string<T> _str;
		std::size_t _size;

		void CheckAndFlush()
		{
			if (_size >= BUFFER_SIZE)
			{
				Flush();
			}
		}

		void Flush()
		{
			if (_size > 0)
			{
				_str.append(_buffer, _size);
				_size = 0;
			}
		}
	};
}

#endif //ELPIDA_STRINGBUFFER_HPP
