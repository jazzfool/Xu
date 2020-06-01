cmake_minimum_required(VERSION 3.16)

project(Xu CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(XU_BUILD_DOCS CACHE ON)
option(XU_ENABLE_TESTS CACHE OFF)

if (${FORCE_GNU_CXX17})
    add_compile_options("--std=c++17")
elseif (${FORCE_MSVC_CXX17})
    add_compile_options("/std:c++17")
endif() 

set(HEADERS
    "include/xu/core/Widget.hpp"
    "include/xu/core/WidgetPtr.hpp"
    "include/xu/core/Definitions.hpp"
    "include/xu/core/Vector2.hpp"
    "include/xu/core/Rect2.hpp"
    "include/xu/core/Size2.hpp"
    "include/xu/core/Point2.hpp"
    "include/xu/core/Context.hpp"
    "include/xu/core/Events.hpp"
    "include/xu/core/Layout.hpp"
    "include/xu/core/RenderData.hpp"
    "include/xu/core/Signal.hpp"
    "include/xu/core/UniqueSlot.hpp"

    "include/xu/kit/BoxStack.hpp"
)

set(SOURCES
    "src/core/Widget.cpp"
    "src/core/Context.cpp"
    "src/core/Layout.cpp"
    "src/core/RenderData.cpp"

    "src/kit/BoxStack.cpp"
)

add_library(Xu)

add_subdirectory(external)

target_sources(Xu PRIVATE ${SOURCES})
target_include_directories(Xu PRIVATE include)

if (${XU_BUILD_DOCS})
    add_custom_target(Xu-Docs ALL
        DEPENDS ${HEADERS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND doxygen Doxyfile
    )
endif(${XU_BUILD_DOCS})

if (${XU_ENABLE_TESTS})
    add_subdirectory(tests)
endif(${XU_ENABLE_TESTS})
