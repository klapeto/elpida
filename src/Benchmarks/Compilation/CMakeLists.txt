project("Elpida Compilation benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compilation benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

if (NOT ELPIDA_DISABLE_LLVM)

    find_package(LLVM REQUIRED CONFIG)

    add_library(elpida-compilation-benchmarks
            Plugin.cpp
            GenerateCppCodeTask.cpp
            CompileWithClangTask.cpp
            ClangCompilationBenchmark.cpp
    )

    target_include_directories(elpida-compilation-benchmarks PRIVATE
            ${LLVM_INCLUDE_DIRS}
    )
    #target_link_libraries(elpida-compilation-benchmarks PUBLIC elpida-core elpida-common-tasks clangBasic clangCodeGen LLVMX86CodeGen musl-lite)
    target_link_libraries(elpida-compilation-benchmarks PUBLIC elpida-core elpida-common-tasks
            clangBasic
            clangCodeGen
            clangDriver
            clangParse
            clangSema
            clangASTMatchers
            clangAPINotes
            clangEdit
            clangSupport
            clangAST
            clangAnalysis
            clangFrontend
            clangLex
            clangSerialization
            LLVMAggressiveInstCombine
            LLVMAnalysis
            LLVMBitReader
            LLVMBitWriter
            LLVMCodeGenTypes
            LLVMCore
            LLVMCoroutines
            LLVMCoverage
            LLVMDemangle
            LLVMExtensions
            LLVMFrontendDriver
            LLVMFrontendHLSL
            LLVMFrontendOpenMP
            LLVMFrontendOffloading
            LLVMHipStdPar
            LLVMipo
            LLVMIRPrinter
            LLVMIRReader
            LLVMInstCombine
            LLVMInstrumentation
            LLVMLTO
            LLVMLinker
            LLVMMC
            LLVMObjCARCOpts
            LLVMObject
            LLVMPasses
            LLVMProfileData
            LLVMScalarOpts
            LLVMSupport
            LLVMTarget
            LLVMTargetParser
            LLVMTransformUtils
            LLVMBinaryFormat
            LLVMWindowsDriver
            LLVMBitstreamReader
            LLVMOption
            LLVMX86CodeGen
            musl-lite)
endif ()