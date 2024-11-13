cmake_minimum_required(VERSION 3.16)

project(ping-stat
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "Tool for given host pinging with visual statistics"
        HOMEPAGE_URL "https://github.com/przemek83/ping-stat")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CODE_COVERAGE "Collect coverage" OFF)

if(CODE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()

set(${PROJECT_NAME}_SOURCES
    src/main.cpp
    src/PlotWidget.h
    src/PlotWidget.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/Pinger.h
    src/Pinger.cpp
    src/Logger.h
    src/Logger.cpp
    src/Constants.h
    src/Constants.cpp
    src/PingData.h
    src/PingerWindows.h
    src/PingerWindows.cpp
    src/PingerLinux.h
    src/PingerLinux.cpp
)

set(UI
    src/MainWindow.ui
)
source_group("UI Files" FILES ${UI})

add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES} ${UI})
target_link_libraries(${PROJECT_NAME}-lib PRIVATE Qt6::Core Qt6::Widgets)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib Qt6::Widgets)

# enable_testing()
# add_subdirectory(test)
