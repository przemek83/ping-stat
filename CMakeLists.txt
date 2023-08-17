cmake_minimum_required(VERSION 3.16)

project(ping-stat
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "Tool for given host pinging with visual statistics"
        HOMEPAGE_URL "https://github.com/przemek83/ping-stat")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/PlotWidget.cpp
    src/MainWindow.cpp
    src/Pinger.cpp
    src/Logger.cpp
    src/Constants.cpp
    src/PingerWindows.cpp
    src/PingerLinux.cpp
)

set(HEADERS
    src/PlotWidget.h
    src/MainWindow.h
    src/Pinger.h
    src/Logger.h
    src/Constants.h
    src/PingData.h
    src/PingerWindows.h
    src/PingerLinux.h
 )

set(UI
    src/MainWindow.ui
)
source_group("UI Files" FILES ${UI})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
