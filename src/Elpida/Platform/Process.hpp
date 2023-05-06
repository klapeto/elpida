//
// Created by klapeto on 6/5/2023.
//

#ifndef ELPIDA_PROCESS_HPP
#define ELPIDA_PROCESS_HPP

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Platform/AnonymousPipe.hpp"

#include <sstream>
#include <thread>
#include <atomic>

namespace Elpida
{

	class Process final
	{
	public:
		void WaitToExit();

		std::istream& GetStdOutput();
		std::istream& GetStdError();

		void Terminate();

		Process(const String& path, const Vector<String>& args);
		Process(const Process&) = delete;
		Process& operator=(const Process&) = delete;
		Process(Process&&) noexcept = delete;
		Process& operator=(Process&&) = delete;
		~Process();
	private:
		std::stringstream _stdOutput;
		std::stringstream _stdError;
		std::thread _stdOutputThread;
		std::thread _stdErrorThread;
		String _path;
		std::atomic<bool> _keepGoing;
		AnonymousPipe _outputPipe;
		AnonymousPipe _errorPipe;
		int _pid;

		static int DoStartProcess(const String& path,
				const Vector<String>& args,
				AnonymousPipe& outputPipe,
				AnonymousPipe& errorPipe);
		void WaitToExitImpl(bool noThrow);
		void ReadAllFromPipe(AnonymousPipe* pipe, std::ostream* stream);
		static bool ReadFromPipe(AnonymousPipe& pipe, char* buffer, std::size_t size, std::size_t& bytesRead);
		static bool WriteToPipe(AnonymousPipe& pipe, char* buffer, std::size_t size, std::size_t& bytesWriten);
	};

} // Elpida

#endif //ELPIDA_PROCESS_HPP
