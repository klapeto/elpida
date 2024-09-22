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

#include "NBodyTask.hpp"

namespace Elpida
{

	static Body<double> Jupiter()
	{
		return Body<double>(
				4.84143144246472090e+00,
				-1.16032004402742839e+00,
				-1.03622044471123109e-01,
				1.66007664274403694e-03 * Body<double>::DaysPerYear,
				7.69901118419740425e-03 * Body<double>::DaysPerYear,
				-6.90460016972063023e-05 * Body<double>::DaysPerYear,
				9.54791938424326609e-04 * Body<double>::SolarMass
		);
	}

	static Body<double> Saturn()
	{
		return Body<double>(
				8.34336671824457987e+00,
				4.12479856412430479e+00,
				-4.03523417114321381e-01,
				-2.76742510726862411e-03 * Body<double>::DaysPerYear,
				4.99852801234917238e-03 * Body<double>::DaysPerYear,
				2.30417297573763929e-05 * Body<double>::DaysPerYear,
				2.85885980666130812e-04 * Body<double>::SolarMass
		);
	}

	static Body<double> Uranus()
	{
		return Body<double>(
				1.28943695621391310e+01,
				-1.51111514016986312e+01,
				-2.23307578892655734e-01,
				2.96460137564761618e-03 * Body<double>::DaysPerYear,
				2.37847173959480950e-03 * Body<double>::DaysPerYear,
				-2.96589568540237556e-05 * Body<double>::DaysPerYear,
				4.36624404335156298e-05 * Body<double>::SolarMass
		);
	}

	static Body<double> Neptune()
	{
		return Body<double>(
				1.53796971148509165e+01,
				-2.59193146099879641e+01,
				1.79258772950371181e-01,
				2.68067772490389322e-03 * Body<double>::DaysPerYear,
				1.62824170038242295e-03 * Body<double>::DaysPerYear,
				-9.51592254519715870e-05 * Body<double>::DaysPerYear,
				5.15138902046611451e-05 * Body<double>::SolarMass
		);
	}

	static Body<double> Sun()
	{
		return Body<double>(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Body<double>::SolarMass);
	}

	void NBodyTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
		_system = NBodySystem<double>({
				Sun(), Jupiter(), Saturn(), Uranus(), Neptune()
		});
	}

	SharedPtr<AbstractTaskData> NBodyTask::Finalize()
	{
		return std::move(_inputData);
	}

	Size NBodyTask::GetProcessedDataSize() const
	{
		return 5;
	}

	void NBodyTask::DoRunImpl()
	{
		Exec([this]()
		{
			_system.Advance(0.01);
		});
	}

	Size NBodyTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> NBodyTask::DoDuplicate() const
	{
		return std::make_unique<NBodyTask>();
	}

	TaskInfo NBodyTask::DoGetInfo() const
	{
		return { "NBody",
				 "Performs NBody simulation",
				 "Iterations",
				 "The iteration process rate",
				 ResultType::Throughput
		};
	}

}