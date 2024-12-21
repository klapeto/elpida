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

#include "RegexTask.hpp"
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{

	void RegexTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		const char last[] = "Nesciunt consequatu Eleos";
		_regex = boost::regex("(N|n)\\w+(u)?n(t)?\\s*co(n){1}se(q)?[a-z]+\\sEleos", boost::regex::ECMAScript | boost::regex::icase);
		_inputData = std::move(inputData);

		if (_inputData->GetSize() <= sizeof(last))
		{
			throw ElpidaException("Too small data for Regex. It needs at least ", sizeof(last));
		}
		std::memcpy(&_inputData->GetData()[_inputData->GetSize() - 1 - sizeof(last)], last, sizeof(last));
	}

	SharedPtr<AbstractTaskData> RegexTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size RegexTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void RegexTask::DoRunImpl()
	{
		auto data = _inputData->GetData();
		auto size = _inputData->GetSize();

		Exec([&]()
		{
			_found = boost::regex_search(data, data + size, _regex);
		});
	}

	Size RegexTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> RegexTask::DoDuplicate() const
	{
		return std::make_unique<RegexTask>();
	}

	TaskInfo RegexTask::DoGetInfo() const
	{
		return { "Regex (Boost)",
				 "Searches text with regex",
				 "Chars",
				 "The character process rate",
				 ResultType::Throughput
		};
	}

}