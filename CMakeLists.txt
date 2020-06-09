cmake_minimum_required(VERSION 3.16)

project(Xu CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(XU_BUILD_DOCS CACHE ON)
option(XU_ENABLE_TESTS CACHE OFF)
option(XU_QUICK_MODULE CACHE ON)

if (${FORCE_GNU_CXX17})
    add_compile_options("--std=c++17")
elseif (${FORCE_MSVC_CXX17})
    add_compile_options("/std:c++17")
endif() 

add_library(Xu)

set(HEADERS
    "include/xu/core/InputEnums.hpp"
    "include/xu/core/InputState.hpp"
    "include/xu/core/Widget.hpp"
    "include/xu/core/WidgetPtr.hpp"
    "include/xu/core/WsiInterface.hpp"
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
    "include/xu/core/Surface.hpp"
    "include/xu/core/VectorPath.hpp"
    "include/xu/core/UniqueSlot.hpp"
    "include/xu/core/Color.hpp"
    "include/xu/core/Theme.hpp"

    "include/xu/kit/BoxStack.hpp"
    "include/xu/kit/Button.hpp"
)

set(SOURCES
    "src/core/Widget.cpp"
    "src/core/Context.cpp"
    "src/core/Layout.cpp"
    "src/core/RenderData.cpp"
    "src/core/Surface.cpp"
    "src/core/Tessellation.cpp"
    "src/core/VectorPath.cpp"

    "src/kit/BoxStack.cpp"
    "src/kit/Button.cpp"
)

if (${XU_QUICK_MODULE})
    set(HEADERS ${HEADERS}
        "include/xu/modules/quick/DarculaTheme.hpp"
        "include/xu/modules/quick/Renderer.hpp"
    )

    set(SOURCES ${SOURCES}
        "src/modules/quick/DarculaTheme.cpp"
        "src/modules/quick/RenderContext.cpp"
    )

    add_library(xu-glad STATIC)
    target_sources(xu-glad PRIVATE "src/modules/quick/ext/glad430/src/glad.c")
    target_include_directories(xu-glad PRIVATE "src/modules/quick/ext/glad430/include")
    target_include_directories(Xu PRIVATE "src/modules/quick/ext/glad430/include")
    target_link_libraries(Xu PUBLIC xu-glad)
endif()

add_subdirectory(external)

target_sources(Xu PRIVATE ${SOURCES})
target_include_directories(Xu PUBLIC include)

if (${BUILD_SHARED_LIBS})
    target_compile_definitions(Xu PRIVATE -DXU_EXPORT)
    target_compile_definitions(Xu PUBLIC -DXU_SHARED=1)
else()
    target_compile_definitions(Xu PUBLIC -DXU_SHARED=0)
endif(${BUILD_SHARED_LIBS})

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
