/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 6/5/2023.
//

#ifndef ELPIDA_PROCESS_HPP
#define ELPIDA_PROCESS_HPP

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Platform/AnonymousPipe.hpp"

#include <filesystem>

namespace Elpida
{

	class Process final
	{
	public:
		void WaitToExit() const;

		AnonymousPipe& GetStdOut();
		AnonymousPipe& GetStdErr();

		void Terminate();

		Process();
		Process(const std::filesystem::path& path, const Vector<String>& args, bool redirectStdOut = false, bool redirectStdErr = false);
		Process(const Process&) = delete;
		Process& operator=(const Process&) = delete;
		Process(Process&&) noexcept;
		Process& operator=(Process&&) noexcept;
		~Process();
	private:
		std::filesystem::path _path;;
		AnonymousPipe _outputPipe;
		AnonymousPipe _errorPipe;
		int _pid;

		static int DoStartProcess(const std::filesystem::path& path,
				const Vector<String>& args,
				AnonymousPipe& outputPipe,
				AnonymousPipe& errorPipe);
		void WaitToExitImpl(bool noThrow) const;
	};

} // Elpida

#endif //ELPIDA_PROCESS_HPP
