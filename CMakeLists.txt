cmake_minimum_required(VERSION 3.17)

project(Xu CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(HEADERS
    include/core/Widget.hpp
    include/core/Definitions.hpp
    include/core/Vector2.hpp
    include/core/Rect2.hpp
    include/core/Size2.hpp
)

set(SOURCES
    src/core/Widget.cpp)

add_library(Xu)

target_sources(Xu PRIVATE ${SOURCES})
target_include_directories(Xu PRIVATE include)
