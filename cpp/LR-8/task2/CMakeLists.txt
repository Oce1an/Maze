cmake_minimum_required(VERSION 3.16)
project(ccc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_library(ArrayHeapLib STATIC
    ArrayHeapLib/ArrayHeap.h
    ArrayHeapLib/ArrayHeap.cpp
)
target_include_directories(ArrayHeapLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ArrayHeapLib)

add_library(ListHeapLib SHARED
    ListHeapLib/ListHeap.h
    ListHeapLib/ListHeap.cpp
)
target_include_directories(ListHeapLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ListHeapLib)
target_compile_definitions(ListHeapLib PRIVATE LISTHEAP_LIBRARY)

if(WIN32)
    set_target_properties(ListHeapLib PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

set(SOURCES
    HeapDemo/main.cpp
    HeapDemo/mainwindow.cpp
)

set(HEADERS
    HeapDemo/mainwindow.h
)

add_executable(ccc ${SOURCES} ${HEADERS})

target_include_directories(ccc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ccc 
    Qt6::Core 
    Qt6::Widgets
    ArrayHeapLib
    ListHeapLib
)

if(WIN32)
    add_custom_command(TARGET ccc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:ListHeapLib>"
            "$<TARGET_FILE_DIR:ccc>"
    )
endif()