project(Elpida-Utilities VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida utilities"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-utilities
        Logging/LogAppender.cpp
        Logging/Logger.cpp

        Event.cpp
        EventsSubscriber.cpp
        EventSubscription.cpp
        Memory.cpp
        NonCopyable.cpp

        NumaMemory.cpp
        NumaMemory.Unix.cpp
        NumaMemory.Windows.cpp

        OffThreadExecutor.cpp

        OsUtilities.cpp
        OsUtilities.Unix.cpp
        OsUtilities.Windows.cpp

        SharedLibrary.cpp
        SharedLibrary.Unix.cpp
        SharedLibrary.Windows.cpp

        SharedLibraryLoader.cpp
        Statistics.cpp

        Timer.cpp
        Timer.Unix.cpp
        Timer.Windows.cpp

        Uuid.cpp
        Uuid.Unix.cpp
        Uuid.Windows.cpp

        ValueUtilities.cpp
)

target_link_libraries(elpida-utilities elpida-common)
target_include_directories(elpida-utilities PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_include_directories(elpida-utilities PRIVATE ${ELPIDA_BINARY_INCLUDE_DIR})