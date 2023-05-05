//
// Created by klapeto on 6/5/2023.
//

#ifndef ELPIDA_PROCESS_HPP
#define ELPIDA_PROCESS_HPP

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"

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
		int _outputPipe[2];
		int _errorPipe[2];
		int _pid;

		void ParentProcessProcedure();
		void ReadAllFromFileDescriptor(int fileDescriptor, std::ostream* stream);
	};

} // Elpida

#endif //ELPIDA_PROCESS_HPP
