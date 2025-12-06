cmake_minimum_required(VERSION 3.20)
project(2.5DEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/Resources")
add_definitions(-DRESOURCE_DIR="${RESOURCE_DIR}")

add_subdirectory(Engine)
add_subdirectory(Game)