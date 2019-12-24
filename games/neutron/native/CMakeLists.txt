cmake_minimum_required(VERSION 3.10)
project(native)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cast-function-type -Wall -O3 -flto -ffast-math -funroll-all-loops -msse4.2 -march=native -mtune=native")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pg -fprofile-arcs -ftest-coverage")

set(SOURCE_FILES main.cpp src/Move.cpp src/FullMove.cpp src/gameutils.cpp src/Board.cpp src/minimax.cpp src/cleaners.cpp)

include_directories(include /home/rigo/shared_folder/node-addon/node_modules/nan/)

add_executable(native ${SOURCE_FILES})
