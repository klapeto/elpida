project("Elpida Qt"
		DESCRIPTION "Elpida Qt Front End"
		HOMEPAGE_URL ${ELPIDA_WEBSITE}
		LANGUAGES CXX C)

set(ELPIDA_QT_SOURCES
		Main.cpp

		MainWindow.hpp
		MainWindow.cpp
		MainWindow.ui

		Layouts/FlowLayout.cpp
		Layouts/FlowLayout.hpp

		Views/OsInfoView/OsInfoView.hpp
		Views/OsInfoView/OsInfoView.cpp
		Views/OsInfoView/OsInfoView.ui

		Views/MemoryInfoView/MemoryInfoView.hpp
		Views/MemoryInfoView/MemoryInfoView.cpp
		Views/MemoryInfoView/MemoryInfoView.ui

		Views/OverheadsInfoView/OverheadsInfoView.hpp
		Views/OverheadsInfoView/OverheadsInfoView.cpp
		Views/OverheadsInfoView/OverheadsInfoView.ui

		Views/CpuInfoView/CpuInfoView.hpp
		Views/CpuInfoView/CpuInfoView.cpp
		Views/CpuInfoView/CpuInfoView.ui

		Views/TopologyNodeView/TopologyNodeView.hpp
		Views/TopologyNodeView/TopologyNodeView.cpp
		Views/TopologyNodeView/TopologyNodeView.ui

		Views/TopologyView/TopologyView.hpp
		Views/TopologyView/TopologyView.cpp
		Views/TopologyView/TopologyView.ui

		Views/BenchmarksView/BenchmarksView.hpp
		Views/BenchmarksView/BenchmarksView.cpp
		Views/BenchmarksView/BenchmarksView.ui

		Images/Images.qrc)


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

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


if (MSYS OR MINGW)
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
target_link_libraries(elpida-qt elpida-core elpida-platform elpida-application Qt5::Core Qt5::Widgets)

#install(TARGETS elpida-qt RUNTIME DESTINATION ${ELPIDA_APP_INSTALL_DIR})
#install(FILES qt.conf DESTINATION ${ELPIDA_APP_INSTALL_DIR})
