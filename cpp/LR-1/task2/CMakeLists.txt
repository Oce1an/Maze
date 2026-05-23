cmake_minimum_required(VERSION 3.16)
project(task2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)
set(SANITIZER_FLAGS "-fsanitize=address -fsanitize=thread -fno-omit-frame-pointer")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    canvas.cpp
    shapes/shape.cpp
    shapes/polygon.cpp
    shapes/triangle.cpp
    shapes/circle.cpp
    shapes/rectangle.cpp
    shapes/square.cpp
    shapes/rhombus.cpp
    shapes/hexagon.cpp
    shapes/star.cpp
    shapes/ellipse.cpp
    animations/rotateanimation.cpp
    animations/moveanimation.cpp
    animations/scaleanimation.cpp
    shapeinfo.cpp
)

set(HEADERS
    mainwindow.h
    canvas.h
    shapes/shape.h
    shapes/polygon.h
    shapes/triangle.h
    shapes/circle.h
    shapes/rectangle.h
    shapes/square.h
    shapes/rhombus.h
    shapes/hexagon.h
    shapes/star.h
    shapes/ellipse.h
    animations/rotateanimation.h
    animations/moveanimation.h
    animations/scaleanimation.h
    shapeinfo.h
)

add_executable(main ${SOURCES} ${HEADERS})

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(main Qt6::Core Qt6::Widgets)