//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_XML_CHARACTERSTREAM_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_XML_CHARACTERSTREAM_HPP

#include <cstddef>
#include <string>
#include <string_view>

namespace Elpida
{

	class CharacterStream
	{
	public:

		static inline int isspace(int c)
		{
			return c == ' ' || (unsigned)c-'\t' < 5;
		}

		template<typename TPredicate>
		bool Skip(TPredicate predicate)
		{
			while (!Eof() && predicate(Current())) Next();

			return Eof();
		}

		bool SkipSpace()
		{
			return Skip([](auto c) { return isspace(c); });
		}

		char Current() const
		{
			return _data[_index];
		}

		bool Next()
		{
			if (Eof()) return false;

			++_index;
			return true;
		}

		template<typename TPredicate>
		std::string_view GetStringViewWhile(TPredicate predicate)
		{
			auto start = _index;
			Skip(predicate);
			return { _data + start, _index - start };
		}

		std::string_view GetStringView(std::size_t begin, std::size_t end)
		{
			return { _data + begin, end - begin };
		}

		template<std::size_t N>
		bool NextCharsAre(const char (&str)[N])
		{
			std::size_t i = 0;
			while (!Eof())
			{
				if (Current() == str[i])
				{
					if (++i == N - 1)
					{
						return true;
					}
				}
				else
				{
					return false;
				}
				Next();
			}
			return false;
		}

		template<std::size_t N>
		bool SkipUntilString(const char (&str)[N])
		{
			std::size_t i = 0;
			while (!Eof())
			{
				if (Current() == str[i])
				{
					if (++i == N - 1)
					{
						return true;
					}
				}
				else
				{
					i = 0;
				}
				Next();
			}
			return false;
		}

		std::size_t Index() const
		{
			return _index;
		}

		bool Eof() const
		{
			return _index > _maxIndex;
		}

		CharacterStream(const char* data, std::size_t size)
			: _data(data), _maxIndex(size - 1), _index(0)
		{

		}

		explicit CharacterStream(const std::string_view& view)
			: CharacterStream(view.data(), view.size())
		{

		}
	private:
		const char* _data;
		std::size_t _maxIndex;
		std::size_t _index;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_XML_CHARACTERSTREAM_HPP
