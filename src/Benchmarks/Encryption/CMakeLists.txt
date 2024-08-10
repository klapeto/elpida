project("Elpida encryption benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida encryption benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_executable(elpida-encryption-benchmarks
        Plugin.cpp
        RSAEncryptTask.cpp
        RSADecryptTask.cpp
        RSAEncryptionBenchmark.cpp
        RSAEncryptionBenchmark.hpp
        RSADecryptionBenchmark.cpp
        RSADecryptionBenchmark.hpp
)

include(FindOpenSSL)

if(MSYS OR MINGW OR MSVC)
    target_link_libraries(elpida-encryption-benchmarks ws2_32 crypto crypt32)
endif()

target_link_libraries(elpida-encryption-benchmarks OpenSSL::SSL elpida-core elpida-platform elpida-common-tasks elpida-entry-point)

install(TARGETS elpida-encryption-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})