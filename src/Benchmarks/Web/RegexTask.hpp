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

#ifndef ELPIDA_REGEXTASK_HPP
#define ELPIDA_REGEXTASK_HPP


#include "Elpida/Core/MicroTask.hpp"
#include "boost/regex.hpp"

namespace Elpida
{

	class RegexTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		RegexTask() = default;
		~RegexTask() override = default;
	protected:
		void DoRunImpl() override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		boost::regex _regex;
		bool _found = false;
	};

}

#endif //ELPIDA_REGEXTASK_HPP
