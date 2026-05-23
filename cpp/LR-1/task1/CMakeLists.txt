cmake_minimum_required(VERSION 3.16)
project(car LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_standard_project_setup()

set(SOURCES
    main.cpp
    src/MainWindow.cpp
    src/MovingRectangle.cpp
    src/Car.cpp
    src/DrawingWidget.cpp
)

set(HEADERS
    hdr/MainWindow.h
    hdr/MovingRectangle.h
    hdr/Car.h
    hdr/DrawingWidget.h
)

add_executable(car ${SOURCES} ${HEADERS})

target_link_libraries(car Qt6::Core Qt6::Widgets)

set_target_properties(car PROPERTIES
    WIN32_EXECUTABLE TRUE
)