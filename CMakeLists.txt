cmake_minimum_required(VERSION 3.16)

project(Xu CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

option(XU_BUILD_DOCS CACHE ON)

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

if (${XU_BUILD_DOCS})
    add_custom_target(Xu-Docs ALL
        DEPENDS ${HEADERS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND doxygen Doxyfile
    )
endif(${XU_BUILD_DOCS})
