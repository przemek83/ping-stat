cmake_minimum_required(VERSION 3.8)

project(ping-stat LANGUAGES CXX VERSION 0.0.1 DESCRIPTION "Host pinging statistics")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
add_definitions( ${QT_DEFINITIONS} )

set(${PROJECT_NAME}_SOURCES
    src/main.cpp
    src/StatsDisplayWidget.cpp
    src/StatsDisplayWidget.h
    src/StatsDisplayWidget.ui
    src/PlotWidget.cpp
    src/PlotWidget.h
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    src/HostChecker.cpp
    src/HostChecker.h
    src/Logger.cpp
    src/Logger.h
    src/Constants.cpp
    src/Constants.h
    )

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)
