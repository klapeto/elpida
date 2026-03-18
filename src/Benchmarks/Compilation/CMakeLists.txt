project("Elpida Compilation benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compilation benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

# this should work but the config files check for executables we delete on docker image
# so we just link with no problems since the libraries are there
#find_package(LLVM REQUIRED CONFIG)
#find_package(Clang REQUIRED)

add_library(elpida-compilation-benchmarks
        Plugin.cpp
        GenerateCppCodeTask.cpp
        CompileWithClangTask.cpp
        ClangCompilationBenchmark.cpp
)

target_include_directories(elpida-compilation-benchmarks PRIVATE
        ${LLVM_INCLUDE_DIRS}
)

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
        LLVMCodeGen
        LLVMGlobalISel
        LLVMSelectionDAG
        LLVMTableGen
        LLVMTableGenBasic
        LLVMTableGenCommon
        LLVMTransformUtils
        LLVMX86TargetMCA
        LLVMX86Desc
        LLVMX86Info
        LLVMMCParser
        LLVMAsmPrinter
        LLVMDebugInfoCodeView
        LLVMDebugInfoDWARF
        LLVMVectorize
        LLVMRemarks
        LLVMAsmParser
        LLVMCFGuard
        LLVMTextAPI
        LLVMMCDisassembler
        LLVMX86CodeGen
        zlibstatic
        musl-lite)

if (WIN32)
    target_link_libraries(elpida-compilation-benchmarks PUBLIC ntdll version)
endif()

if (LINUX)
    target_link_libraries(elpida-compilation-benchmarks PUBLIC dl)
endif ()

if (ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES)
    install(TARGETS elpida-compilation-benchmarks ARCHIVE)
endif ()