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

if (CMAKE_CROSSCOMPILING)
	set(QT_HOST_PATH "/usr")
	set(QT_HOST_PATH_CMAKE_DIR "/lib/${CMAKE_HOST_SYSTEM_PROCESSOR}-linux-gnu/cmake")
	set(QT_REQUIRE_HOST_PATH_CHECK ON)
endif ()

find_package(Qt6 COMPONENTS Widgets Charts)

if (NOT Qt6Core_FOUND)
	message("Disabling QT front end because Qt5 libraries (Core/Widgets/Charts) were not found")
	return()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#qt_standard_project_setup()

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
			FILE_DESCRIPTION "Elpida QT"
	)
endif()

qt_add_executable(elpida-qt ${ELPIDA_QT_SOURCES})

if (MSYS OR MINGW)
	target_link_options(elpida-qt PRIVATE -Wl,-subsystem,windows)
endif()

target_include_directories(elpida-qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(elpida-qt PUBLIC elpida-application elpida-shared Qt6::Core Qt6::Widgets Qt6::Charts)

install(TARGETS elpida-qt RUNTIME DESTINATION bin)
install(FILES qt.conf DESTINATION bin)
