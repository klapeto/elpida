//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 21/9/2024.
//

#include "GenerateLoremIpsum.hpp"

#include "Elpida/Core/ValueUtilities.hpp"

#include <random>
#include <algorithm>
#include <cstring>

namespace Elpida
{
	static const char* words[] = { "lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit",
								   "sed", "do", "eiusmod", "tempor", "incididunt", "ut", "labore", "et", "dolore",
								   "magna", "aliqua", "ut", "enim", "ad", "minim", "veniam", "quis", "nostrud",
								   "exercitation", "ullamco", "laboris", "nisi", "aliquip", "ex", "ea",
								   "commodo", "consequat", "duis", "aute", "irure", "in", "reprehenderit", "voluptate",
								   "velit","esse", "cillum", "eu", "fugiat", "nulla",
								   "pariatur", "excepteur", "sint", "occaecat", "cupidatat", "non", "proident",
								   "sunt", "culpa", "qui", "officia", "deserunt", "mollit", "anim", "id", "est","laborum",
								   "sed", "perspiciatis", "unde", "omnis", "iste", "natus", "error", "voluptatem",
								   "accusantium", "doloremque", "laudantium", "totam", "rem", "aperiam", "eaque","ipsa",
								   "quae", "ab", "illo", "inventore", "veritatis", "quasi", "architecto", "beatae",
								   "vitae", "dicta", "explicabo", "nemo", "ipsam", "quia",
								   "voluptas", "aspernatur", "aut", "odit", "fugit", "consequuntur",
								   "magni", "dolores", "eos", "ratione", "sequi", "nesciunt", "neque",
								   "porro", "quisquam", "dolorem", "adipisci", "numquam","eius", "modi",
								   "tempora", "incidunt", "magnam", "aliquam", "quaerat", "minima", "nostrum", "exercitationem",
								   "ullam", "corporis", "suscipit", "laboriosam", "nisi", "aliquid", "ex", "ea",
								   "commodi", "consequatur", "quis", "autem", "vel", "eum", "iure",
								   "quam", "nihil", "molestiae", "illum", "quo" };

	void GenerateLoremIpsum::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());

		_outputData->Allocate(_size);
	}

	SharedPtr<AbstractTaskData> GenerateLoremIpsum::Finalize()
	{
		return std::move(_outputData);
	}

	ConcurrencyMode GenerateLoremIpsum::GetAllowedConcurrency() const
	{
		return ConcurrencyMode::None;
	}

	Size GenerateLoremIpsum::GetProcessedDataSize() const
	{
		return _size;
	}

	GenerateLoremIpsum::GenerateLoremIpsum(std::size_t size)
			:_size(size)
	{

	}

	void GenerateLoremIpsum::DoRun()
	{
		const std::size_t initialWords = 19;
		const std::size_t sentenceMinWordCount = 5;
		const std::size_t sentenceMaxWordCount = 15;
		const std::size_t paragraphMinSentenceCount = 5;
		const std::size_t paragraphMaxSentenceCount = 10;

		auto data = _outputData->GetData();
		auto generator = std::mt19937(12345); // NOLINT(*-msc51-cpp)

		std::size_t i = 0;
		std::size_t initialWordIndex = 0;
		while (i < _size && initialWordIndex < initialWords)
		{
			auto nextWord = words[initialWordIndex++];
			auto wordSize = std::strlen(nextWord);

			auto copySize = std::min(wordSize, _size - i);
			std::memcpy(data + i, nextWord, copySize);

			if (i == 0)
			{
				data[i] = 'L';
			}

			i += copySize;

			if (i < _size)
			{
				data[i++] = ' ';
			}
		}

		if (i > 0)
		{
			data[i - 1] = '.';
			if (i < _size)
			{
				data[i++] = ' ';
			}
		}

		auto nextSentence = [&]() -> std::size_t { return generator() % (sentenceMaxWordCount - sentenceMinWordCount + 1) + sentenceMinWordCount; };
		auto nextParagraph = [&]() -> std::size_t { return generator() % (paragraphMaxSentenceCount - paragraphMinSentenceCount + 1) + paragraphMinSentenceCount; };

		std::size_t sentenceCount = 1;
		std::size_t wordCount = 0;
		std::size_t nextSentenceWordCount = nextSentence();
		std::size_t nextParagraphSentenceCount = nextParagraph();

		for (; i < _size;)
		{
			auto nextWordIndex = generator() % ValueUtilities::GetArrayLength(words);
			auto nextWord = words[nextWordIndex];

			auto wordSize = std::strlen(nextWord);

			auto copySize = std::min(wordSize, _size - i);
			std::memcpy(data + i, nextWord, copySize);

			i += copySize;

			if (wordCount == 0)
			{
				data[i - copySize] = std::toupper(data[i - copySize]);
			}

			if (i < _size)
			{
				data[i++] = ' ';
			}

			if (++wordCount >= nextSentenceWordCount)
			{
				wordCount = 0;
				nextSentenceWordCount = nextSentence();

				if (i > 0)
				{
					data[i - 1] = '.';
					if (i < _size)
					{
						data[i++] = ' ';
					}
				}

				if (++sentenceCount >= nextParagraphSentenceCount)
				{
					sentenceCount = 0;
					nextParagraphSentenceCount = nextParagraph();

					if (i < _size)
					{
						data[i++] = '\n';
						if (i < _size)
						{
							data[i++] = '\n';
						}
					}
				}
			}
		}
	}

	std::unique_ptr<Task> GenerateLoremIpsum::DoDuplicate() const
	{
		return std::make_unique<GenerateLoremIpsum>(_size);
	}

	TaskInfo GenerateLoremIpsum::DoGetInfo() const
	{
		return {
				"Generate Lorem Ipsum text",
				"Generates random Lorem Ipsum text",
				"chars",
				"The rate of the generation",
				ResultType::Throughput,
				false
		};
	}
} // Elpida