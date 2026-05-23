cmake_minimum_required(VERSION 3.16)
project(task1_5_combined)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(SOURCES
    main.cpp
    task1widget.cpp
    task2widget.cpp
    task3widget.cpp
    vectordemowidget.cpp
)

set(HEADERS
    vector.h
    pair.h
    task1widget.h
    task2widget.h
    task3widget.h
    vectordemowidget.h
)

add_executable(comp ${SOURCES} ${HEADERS})

target_include_directories(comp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(comp Qt6::Core Qt6::Widgets)