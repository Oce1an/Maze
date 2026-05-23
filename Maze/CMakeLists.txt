cmake_minimum_required(VERSION 3.16)

project(MazePathfinder VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 QUIET COMPONENTS Widgets)
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MazeWidget.cpp
    src/Maze.cpp
    src/generators/MazeGenerator.cpp
    src/generators/DepthFirstGenerator.cpp
    src/generators/PrimGenerator.cpp
    src/generators/KruskalGenerator.cpp
    src/solvers/PathSolver.cpp
    src/solvers/SolverUtils.cpp
    src/solvers/BfsSolver.cpp
    src/solvers/DfsSolver.cpp
    src/solvers/DijkstraSolver.cpp
    src/solvers/AStarSolver.cpp
)

set(HEADERS
    src/MainWindow.h
    src/MazeWidget.h
    src/Maze.h
    src/generators/MazeGenerator.h
    src/generators/DepthFirstGenerator.h
    src/generators/PrimGenerator.h
    src/generators/KruskalGenerator.h
    src/solvers/PathSolver.h
    src/solvers/BfsSolver.h
    src/solvers/DfsSolver.h
    src/solvers/DijkstraSolver.h
    src/solvers/AStarSolver.h
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(MazePathfinder ${SOURCES} ${HEADERS})
    target_link_libraries(MazePathfinder PRIVATE Qt6::Widgets)
else()
    add_executable(MazePathfinder ${SOURCES} ${HEADERS})
    target_link_libraries(MazePathfinder PRIVATE Qt5::Widgets)
endif()

target_include_directories(MazePathfinder PRIVATE src)

if(WIN32)
    set_target_properties(MazePathfinder PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
