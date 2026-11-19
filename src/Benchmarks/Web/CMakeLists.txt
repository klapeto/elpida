project("Elpida Web benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida web benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

# The real-world inputs of the Json, XML and Regex benchmarks, embedded so every platform processes the same bytes
# (sources and licenses in Assets/README.md)
set(WEB_BENCHMARK_EMBEDDED_SOURCES)
foreach (ASSET JsonBenchmarkDocument.json XmlBenchmarkDocument.xml RegexBenchmarkText.txt)
    get_filename_component(ASSET_NAME "${ASSET}" NAME_WLE)
    set(ASSET_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/Assets/${ASSET}")
    set(ASSET_CPP "${CMAKE_CURRENT_BINARY_DIR}/${ASSET_NAME}.cpp")
    add_custom_command(
            OUTPUT "${ASSET_CPP}"
            COMMAND ${CMAKE_COMMAND}
            -DINPUT=${ASSET_INPUT}
            -DOUTPUT=${ASSET_CPP}
            -DNAMESPACE=Elpida::Benchmarks::Web
            -DNAME=${ASSET_NAME}
            -P "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake-modules/EmbedFile.cmake"
            DEPENDS "${ASSET_INPUT}" "${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake-modules/EmbedFile.cmake"
            COMMENT "Embedding ${ASSET}"
            VERBATIM)
    list(APPEND WEB_BENCHMARK_EMBEDDED_SOURCES "${ASSET_CPP}")
endforeach ()

add_library(elpida-web-benchmarks
        ${WEB_BENCHMARK_EMBEDDED_SOURCES}
        Plugin.cpp
        XmlParseBenchmark.cpp
        ParseJsonTask.cpp
        EmbeddedDocumentTask.cpp
        JsonParseBenchmark.cpp
        Base64.cpp
        Base64EncodeTask.cpp
        Base64EncodeBenchmark.cpp
        Base64DecodeTask.cpp
        Base64DecodeBenchmark.cpp
        RegexTask.cpp
        RegexBenchmark.cpp
)

target_link_libraries(elpida-web-benchmarks PUBLIC elpida-core elpida-common-tasks boost_regex musl-lite)

if (ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES)
    install(TARGETS elpida-web-benchmarks ARCHIVE)
endif ()