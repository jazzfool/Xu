add_executable(xu-test-app "")

target_sources(xu-test-app PRIVATE main.cpp)
target_link_libraries(xu-test-app PRIVATE Xu)
target_include_directories(xu-test-app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")