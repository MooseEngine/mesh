cmake_minimum_required(VERSION 3.10)
project(MooseEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(moose_engine
	src/main.cpp
	src/engine/Engine.cpp
	src/graphics/Camera.cpp
)

find_package(SDL2 REQUIRED)

# SDL2
# SDL2::SDL2 is the core library
# SDL2::SDL2main provides main entry-point on some platforms
target_link_libraries(moose_engine
	PRIVATE
		SDL2::SDL2
		SDL2::SDL2main
)
