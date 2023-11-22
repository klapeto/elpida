//
// Created by klapeto on 22/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_XML_CHARACTERSTREAM_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_XML_CHARACTERSTREAM_HPP

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>
namespace Elpida
{

	class CharacterStream
	{
	public:

		template<typename TPredicate>
		bool Skip(TPredicate predicate)
		{
			while (!Eof() && predicate(Current())) Next();

			return Eof();
		}

		bool SkipSpace()
		{
			return Skip([](auto c) { return std::isspace(c); });
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
			return std::string_view(_data + start, _index - start);
		}

		bool Back()
		{
			if (_index == 0) return false;

			--_index;
			return true;
		}

		bool Eof() const
		{
			return _index == _maxIndex;
		}

		CharacterStream(const char* data, std::size_t size)
			: _data(data), _maxIndex(size - 1), _index(0)
		{

		}
	private:
		const char* _data;
		std::size_t _maxIndex;
		std::size_t _index;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_XML_CHARACTERSTREAM_HPP
