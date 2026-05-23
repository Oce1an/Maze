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
    mainwindow.cpp
)

set(HEADERS
    hashtable.h
    mainwindow.h
)

add_executable(c ${SOURCES} ${HEADERS})

target_include_directories(c PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(c Qt6::Core Qt6::Widgets)