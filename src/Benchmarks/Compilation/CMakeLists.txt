project("Elpida Compilation benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Compilation benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED)

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
#target_link_libraries(elpida-compilation-benchmarks PUBLIC elpida-core elpida-common-tasks clangBasic clangCodeGen LLVMX86CodeGen musl-lite)
#target_include_directories(elpida-compilation-benchmarks PRIVATE ${LLVM_INCLUDE_DIRECTORIES})
target_link_libraries(elpida-compilation-benchmarks PUBLIC elpida-core elpida-common-tasks
        clangCodeGen
        clangFrontend
        clangDriver
        LLVMWindowsDriver
        clangParse
        LLVMOption
        clangSerialization
        clangSema
        clangAnalysis
        clangASTMatchers
        clangAPINotes
        clangEdit
        clangAST
        clangLex
        clangBasic
        clangSupport
        LLVMCoverage
        LLVMFrontendDriver
        LLVMFrontendHLSL
        LLVMLTO
        LLVMExtensions
        LLVMPasses
        LLVMCoroutines
        LLVMHipStdPar
        LLVMipo
        LLVMFrontendOpenMP
        LLVMFrontendOffloading
        LLVMObjectYAML
        LLVMFrontendAtomic
        LLVMFrontendDirective
        LLVMLinker
        LLVMVectorize
        LLVMSandboxIR
        LLVMX86CodeGen
        LLVMIRPrinter
        LLVMInstrumentation
        LLVMAsmPrinter
        LLVMCFGuard
        LLVMGlobalISel
        LLVMSelectionDAG
        LLVMCodeGen
        LLVMObjCARCOpts
        LLVMScalarOpts
        LLVMAggressiveInstCombine
        LLVMInstCombine
        LLVMTarget
        LLVMTransformUtils
        LLVMCGData
        LLVMBitWriter
        LLVMAnalysis
        LLVMProfileData
        LLVMSymbolize
        LLVMDebugInfoGSYM
        LLVMDebugInfoPDB
        LLVMDebugInfoCodeView
        LLVMDebugInfoMSF
        LLVMDebugInfoBTF
        LLVMDebugInfoDWARF
        LLVMDebugInfoDWARFLowLevel
        LLVMObject
        LLVMIRReader
        LLVMBitReader
        LLVMAsmParser
        LLVMCore
        LLVMRemarks
        LLVMBitstreamReader
        LLVMMCParser
        LLVMTextAPI
        LLVMX86Desc
        LLVMCodeGenTypes
        LLVMMCDisassembler
        LLVMX86Info
        LLVMMC
        LLVMBinaryFormat
        LLVMTargetParser
        LLVMSupport
        LLVMDemangle
        musl-lite)


