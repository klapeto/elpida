//
// Created by klapeto on 6/5/2023.
//

#ifndef ELPIDA_PROCESS_HPP
#define ELPIDA_PROCESS_HPP

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Platform/AnonymousPipe.hpp"

namespace Elpida
{

	class Process final
	{
	public:
		void WaitToExit() const;

		const AnonymousPipe& GetStdOut() const;
		const AnonymousPipe& GetStdErr() const;

		void Terminate();

		Process();
		Process(const String& path, const Vector<String>& args, bool redirectStdOut = false, bool redirectStdErr = false);
		Process(const Process&) = delete;
		Process& operator=(const Process&) = delete;
		Process(Process&&) noexcept;
		Process& operator=(Process&&) noexcept;
		~Process();
	private:
		String _path;
		AnonymousPipe _outputPipe;
		AnonymousPipe _errorPipe;
		int _pid;

		static int DoStartProcess(const String& path,
				const Vector<String>& args,
				AnonymousPipe& outputPipe,
				AnonymousPipe& errorPipe);
		void WaitToExitImpl(bool noThrow) const;
	};

} // Elpida

#endif //ELPIDA_PROCESS_HPP
