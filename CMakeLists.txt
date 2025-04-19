cmake_minimum_required(VERSION 3.10)
project(MooseEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(moose_engine
	src/main.cpp
	src/core/Mat4.cpp
	src/core/Vec4.cpp
)
