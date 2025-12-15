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
// Created by klapeto on 15/12/2024.
//

#include "CompileWithClangTask.hpp"

#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/MemoryBuffer.h>
#include <clang/Lex/PreprocessorOptions.h>

namespace Elpida
{
	void CompileWithClangTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_input = std::move(inputData);

		_sourceBuffer = llvm::MemoryBuffer::getMemBuffer(reinterpret_cast<const char*>(_input->GetData()));
		_compiler.getInvocation().getPreprocessorOpts().clearRemappedFiles();
		_compiler.getInvocation().getPreprocessorOpts().addRemappedFile("test.cpp", _sourceBuffer.get());
	}

	SharedPtr<AbstractTaskData> CompileWithClangTask::Finalize()
	{
		return std::move(_input);
	}

	Size CompileWithClangTask::GetProcessedDataSize() const
	{
		return 1;
	}

	void CompileWithClangTask::DoRunImpl()
	{
		Exec([&]()
		{
			_compiler.setOutputStream(_compiler.createNullOutputFile());
			_compiler.ExecuteAction(_action);
		});
	}

	TaskInfo CompileWithClangTask::DoGetInfo() const
	{
		return TaskInfo("Clang compilation",
				"Compiles a file with clang",
				"Files",
				"The compilation file throughput",
				ResultType::Throughput,
				true);
	}

	Size CompileWithClangTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> CompileWithClangTask::DoDuplicate() const
	{
		return std::make_unique<CompileWithClangTask>();
	}

	CompileWithClangTask::CompileWithClangTask()
	{
		_diagnosticIds = new clang::DiagnosticIDs();
		_diagnosticOptions = clang::DiagnosticOptions();
		_diagnosticsEngine = std::make_unique<clang::DiagnosticsEngine>(_diagnosticIds, _diagnosticOptions, &_diagnosticConsumer, false);

		clang::CompilerInvocation::CreateFromArgs(_compiler.getInvocation(), { "-O3", "test.cpp" }, *_diagnosticsEngine);
		_compiler.getInvocation().getTargetOpts().Triple = "x86_64-none-none";
		_compiler.getInvocation().resetNonModularOptions();

		_compiler.setDiagnostics(new clang::DiagnosticsEngine(
				_diagnosticIds,
				_diagnosticOptions,
				new clang::TextDiagnosticPrinter(llvm::errs(), _diagnosticOptions))
		);
	}
} // Elpida