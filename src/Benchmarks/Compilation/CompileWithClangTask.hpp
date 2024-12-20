//
// Created by klapeto on 15/12/2024.
//

#ifndef ELPIDA_COMPILEWITHCLANGTASK_HPP
#define ELPIDA_COMPILEWITHCLANGTASK_HPP

#include <memory>
#include <optional>

#include "Elpida/Core/MicroTask.hpp"

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/CodeGen/CodeGenAction.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>

namespace Elpida
{
	class CompileWithClangTask: public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		CompileWithClangTask();
		~CompileWithClangTask() override  = default;
	protected:
		void DoRunImpl() override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _input;
		clang::CompilerInstance _compiler;
		std::shared_ptr<clang::CompilerInvocation> _invocation;
		llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> _diagnosticIds;
		llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> _diagnosticOptions;
		std::unique_ptr<clang::DiagnosticsEngine> _diagnosticsEngine;
		clang::EmitObjAction _action;
		std::unique_ptr<llvm::MemoryBuffer> _sourceBuffer;
	};

} // Elpida

#endif //ELPIDA_COMPILEWITHCLANGTASK_HPP
