cmake_minimum_required(VERSION 3.15)
project(SPLflix)

set(CMAKE_CXX_STANDARD 11)

add_executable(SPLflix src/Main.cpp src/Session.cpp src/Watchable.cpp src/Movie.cpp src/Episode.cpp)