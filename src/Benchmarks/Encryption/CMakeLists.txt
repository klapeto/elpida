project("Elpida encryption benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida encryption benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_executable(elpida-encryption-benchmarks
        Plugin.cpp
        RSAEncryptTask.cpp
        RSADecryptTask.cpp
        RSAEncryptionBenchmark.cpp
        RSADecryptionBenchmark.cpp
        AESEncryptTask.cpp
        Utilities.cpp
        AESDecryptTask.cpp
        AESEncryptionBenchmark.cpp
        AESDecryptionBenchmark.cpp
)

include(FindOpenSSL)

if(MSYS OR MINGW OR MSVC)
    target_link_libraries(elpida-encryption-benchmarks PRIVATE crypto crypt32 ws2_32 zlibstatic)
endif()

target_link_libraries(elpida-encryption-benchmarks PRIVATE musl OpenSSL::SSL elpida-core elpida-platform elpida-common-tasks elpida-entry-point)

install(TARGETS elpida-encryption-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})