/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 29/9/2024.
//

#include "InfoGetter.hpp"

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Platform/Process.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	std::string InfoGetter::GetData() const
	{
		auto pathString = _benchmarksPath.string();
		ValueUtilities::QuoteString(pathString);

		auto suffix = _benchmarksSuffix;
		ValueUtilities::QuoteString(suffix);

		auto executablePath = _infoGetterPath.empty() ? OsUtilities::GetExecutableDirectory() / "elpida-info-dumper" : _infoGetterPath;
		Vector<String> args;
		args.push_back("--benchmarks-directory=" + pathString);
		if (!_benchmarksSuffix.empty())
		{
			args.push_back("--benchmarks-suffix=" + suffix);
		}

		if (_noThreadPinning)
		{
			args.emplace_back("--no-thread-pinning");
		}

		Process process(executablePath, args, true, true);
		AsyncPipeReader stdOut(process.GetStdOut());
		AsyncPipeReader stdErr(process.GetStdErr());

		stdOut.StartReading();
		stdErr.StartReading();
		try
		{
			process.WaitToExit();
			stdOut.WaitForDrain();
			return stdOut.GetString();
		}
		catch (...)
		{
			stdErr.WaitForDrain();
			auto err = stdErr.GetString();
			if (err.empty())
			{
				throw;
			}
			else
			{
				throw ElpidaException("Info dumper process failed with error: ", err);
			}
		}
	}

	InfoGetter::InfoGetter():
		_noThreadPinning(false)
	{

	}
} // Elpida