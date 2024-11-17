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

if(MSYS OR MINGW OR MSVC)
    # hack to make sure it is linked correctly. Cmake messes the order when crypto is defined and links ws2_32 before crypto
    # `crypto` target is used to ensure is is built before building this
    target_link_libraries(elpida-encryption-benchmarks PRIVATE crypto ${CRYPTO_CUSTOM_LIB_PATH} ws2_32 crypt32 zlibstatic)
else()
    target_link_libraries(elpida-encryption-benchmarks PRIVATE crypto ${CRYPTO_CUSTOM_LIB_PATH} zlibstatic)
endif()

target_link_libraries(elpida-encryption-benchmarks PRIVATE elpida-core elpida-platform elpida-common-tasks elpida-entry-point)

install(TARGETS elpida-encryption-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})