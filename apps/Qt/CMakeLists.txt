project("Elpida Qt"
		DESCRIPTION "Elpida Qt Front End"
		LANGUAGES CXX C)

set(ELPIDA_QT_SOURCES
		Main.cpp

		MainWindow.hpp
		MainWindow.cpp
		MainWindow.ui

		Layouts/FlowLayout.cpp
		Layouts/FlowLayout.hpp

		Views/ConfigurationViews/ConfigurationView.cpp
		Views/ConfigurationViews/ConfigurationView.hpp

		Views/OsInfoView/OsInfoView.hpp
		Views/OsInfoView/OsInfoView.cpp
		Views/OsInfoView/OsInfoView.ui

		Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp
		Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.cpp
		Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.ui

		Views/MemoryInfoView/MemoryInfoView.hpp
		Views/MemoryInfoView/MemoryInfoView.cpp
		Views/MemoryInfoView/MemoryInfoView.ui

		Views/TimingInfoView/TimingInfoView.hpp
		Views/TimingInfoView/TimingInfoView.cpp
		Views/TimingInfoView/TimingInfoView.ui

		Views/CpuInfoView/CpuInfoView.hpp
		Views/CpuInfoView/CpuInfoView.cpp
		Views/CpuInfoView/CpuInfoView.ui

		Views/TopologyNodeView/TopologyNodeView.hpp
		Views/TopologyNodeView/TopologyNodeView.cpp
		Views/TopologyNodeView/TopologyNodeView.ui

		Views/TopologyView/TopologyView.hpp
		Views/TopologyView/TopologyView.cpp
		Views/TopologyView/TopologyView.ui

		Views/CustomBenchmarkView/CustomBenchmarkView.hpp
		Views/CustomBenchmarkView/CustomBenchmarkView.cpp
		Views/CustomBenchmarkView/CustomBenchmarkView.ui

		Views/FullBenchmarkView/FullBenchmarkView.hpp
		Views/FullBenchmarkView/FullBenchmarkView.cpp
		Views/FullBenchmarkView/FullBenchmarkView.ui

		Views/CustomBenchmarkConfigurationView/CustomBenchmarkConfigurationView.hpp
		Views/CustomBenchmarkConfigurationView/CustomBenchmarkConfigurationView.cpp
		Views/CustomBenchmarkConfigurationView/CustomBenchmarkConfigurationView.ui

		Views/ConfigurationViews/BooleanConfigurationView/BooleanConfigurationView.hpp
		Views/ConfigurationViews/BooleanConfigurationView/BooleanConfigurationView.cpp
		Views/ConfigurationViews/BooleanConfigurationView/BooleanConfigurationView.ui

		Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.hpp
		Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.cpp
		Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.ui

		Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.hpp
		Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.cpp
		Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.ui

		Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.hpp
		Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.cpp
		Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.ui

		Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.hpp
		Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.cpp
		Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.ui

		Views/CustomBenchmarkResultsView/CustomBenchmarkResultsView.hpp
		Views/CustomBenchmarkResultsView/CustomBenchmarkResultsView.cpp
		Views/CustomBenchmarkResultsView/CustomBenchmarkResultsView.ui

		Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.hpp
		Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.cpp
		Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.ui

        Views/MemoryBenchmarkView/MemoryBenchmarkView.hpp
		Views/MemoryBenchmarkView/MemoryBenchmarkView.cpp
		Views/MemoryBenchmarkView/MemoryBenchmarkView.ui

		Images/Images.qrc

        ConfigurationViewPool.cpp
		QtMessageService.cpp
		QtThreadQueue.cpp
		QtSettingsService.cpp
		QtPathsService.cpp
		QtPathsService.hpp
		QtDesktopService.cpp
        QtDesktopService.hpp
)


if (MSVC)
	set(QT_MISSING True)

	# Huge thanks to https://stackoverflow.com/a/47723189
	# look for user-registry pointing to qtcreator
	get_filename_component(QT_BIN [HKEY_CURRENT_USER\\Software\\Classes\\Applications\\QtProject.QtCreator.cpp\\shell\\Open\\Command] PATH)

	# get root path so we can search for 5.3, 5.4, 5.5, etc
	string(REPLACE "/Tools" ";" QT_BIN "${QT_BIN}")
	list(GET QT_BIN 0 QT_BIN)
	file(GLOB QT_VERSIONS "${QT_BIN}/5.*")
	list(SORT QT_VERSIONS)

	# assume the latest version will be last alphabetically
	list(REVERSE QT_VERSIONS)

	list(GET QT_VERSIONS 0 QT_VERSION)

	# fix any double slashes which seem to be common
	string(REPLACE "//" "/"  QT_VERSION "${QT_VERSION}")

	if (${MSVC_TOOLSET_VERSION} EQUAL 120)
		set(QT_MSVC 2013)
	elseif(${MSVC_TOOLSET_VERSION} EQUAL 140)
		set(QT_MSVC 2015)
	elseif(${MSVC_TOOLSET_VERSION} EQUAL 141)
		set(QT_MSVC 2017)
	elseif(${MSVC_TOOLSET_VERSION} EQUAL 142)
		set(QT_MSVC 2019)
	elseif(${MSVC_TOOLSET_VERSION} EQUAL 143)
		set(QT_MSVC 2022)
	else()
		message(FATAL_ERROR "Unsupported MSVC version")
	endif()

	# check for 64-bit os
	# may need to be removed for older compilers as it wasn't always offered
	if(CMAKE_SYSTEM_PROCESSOR MATCHES 64)
		set(QT_MSVC "${QT_MSVC}_64")
	endif()
	set(QT_PATH "${QT_VERSION}/msvc${QT_MSVC}")
	set(QT_MISSING False)

	# use Qt_DIR approach so you can find Qt after cmake has been invoked
	if(NOT QT_MISSING)
		message("-- Qt found: ${QT_PATH}")
		set(Qt5_DIR "${QT_PATH}/lib/cmake/Qt5/")
		set(Qt5Test_DIR "${QT_PATH}/lib/cmake/Qt5Test")
	endif()

endif()

if (ELPIDA_DEBUG_BUILD EQUAL 1)
	add_definitions(-DTASK_BATCH_DEBUG_DIR="${TASK_BATCH_DEBUG_DIR}")
endif()

find_package(Qt5 COMPONENTS Widgets Charts)

if (NOT Qt5Core_FOUND)
	message("Disabling QT front end because Qt5 libraries (Core/Widgets/Charts) were not found")
	return()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()

if (MSYS OR MINGW OR MSVC)
	generate_product_version(
			ELPIDA_QT_SOURCES
			NAME ${PROJECT_NAME}
			ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
			VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
			VERSION_MINOR ${ELPIDA_VERSION_MINOR}
			VERSION_PATCH ${ELPIDA_VERSION_REVISION}
			VERSION_REVISION ${ELPIDA_VERSION_BUILD}
			COMPANY_NAME ${ELPIDA_COMPANY_NAME}
			FILE_DESCRIPTION ${PROJECT_DESCRIPTION}
	)
endif()

add_executable(elpida-qt ${ELPIDA_QT_SOURCES})

if (MSYS OR MINGW)
	target_link_options(elpida-qt PRIVATE -Wl,-subsystem,windows)
endif()

target_include_directories(elpida-qt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(elpida-qt PRIVATE elpida-application elpida-shared Qt5::Core Qt5::Widgets Qt5::Charts)

install(TARGETS elpida-qt RUNTIME DESTINATION bin)
install(FILES qt.conf DESTINATION bin)
