cmake_minimum_required(VERSION 2.8)

project(CartesianMeshRuntime)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

add_executable(cartesianmeshruntime main.cpp)
target_link_libraries(cartesianmeshruntime cmr)

add_subdirectory(lib)
add_subdirectory(tests)
