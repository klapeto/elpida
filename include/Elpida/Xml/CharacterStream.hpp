//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_XML_CHARACTERSTREAM_HPP
#define ELPIDA_XML_CHARACTERSTREAM_HPP

#include <cstddef>
#include <string_view>

namespace Elpida
{
	class CharacterStream
	{
	public:
		static int IsSpace(const int c)
		{
			return c == ' ' || static_cast<unsigned>(c) - '\t' < 5;
		}

		template <typename TPredicate>
		bool Skip(TPredicate predicate)
		{
			while (!Eof() && predicate(Current())) Next();

			return Eof();
		}

		bool SkipSpace()
		{
			return Skip([](auto c)
			{
				return IsSpace(c);
			});
		}

		char Current() const
		{
			if (!Eof())
			{
				return _data[_index];
			}
			return 0;
		}

		bool Next()
		{
			if (Eof()) return false;

			++_index;
			UpdateEofFlag();
			return !_eof;
		}

		template <typename TPredicate>
		std::string_view GetStringViewWhile(TPredicate predicate)
		{
			const auto start = _index;
			Skip(predicate);
			return {_data + start, _index - start};
		}

		std::string_view GetStringView()
		{
			return GetStringView(_index, _maxIndex);
		}

		std::string_view GetStringView(const std::size_t begin, const std::size_t end)
		{
			if (begin <= _maxIndex && end <= _maxIndex)
			{
				return {_data + begin, _data + end + 1};
			}
			return {};
		}

		template <std::size_t N>
		bool ConsumeNextChars(const char (&str)[N])
		{
			std::size_t i = 0;
			while (!Eof())
			{
				if (Current() == str[i])
				{
					if (++i == N - 1)
					{
						Next();
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

		template <std::size_t N>
		bool ConsumeNextCharsCond(const char (&str)[N])
		{
			for (std::size_t i = 0; i < N - 1; ++i)
			{
				if (str[i] != FutureChar(i)) return false;
			}
			_index += N - 1;
			UpdateEofFlag();
			return true;
		}

		template <std::size_t N>
		bool SkipUntilString(const char (&str)[N])
		{
			std::size_t i = 0;

			while (!Eof())
			{
				if (Current() == str[i])
				{
					if (++i == N - 1)
					{
						Next();
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
			return _eof;
		}

		std::size_t AvailableCharacters() const
		{
			return _maxIndex - _index;
		}

		char FutureChar(const std::size_t i) const
		{
			if (_index + i <= _maxIndex)
			{
				return _data[_index + i];
			}
			return 0;
		}

		char Char(const std::size_t i) const
		{
			if (i <= _maxIndex)
			{
				return _data[i];
			}
			return 0;
		}

		CharacterStream(const char* data, const std::size_t size)
			: _data(data), _maxIndex(size - 1), _index(0), _eof(size == 0)
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
		bool _eof;

		void UpdateEofFlag()
		{
			_eof = _index > _maxIndex;
		}
	};
} // Elpida

#endif //ELPIDA_XML_CHARACTERSTREAM_HPP
