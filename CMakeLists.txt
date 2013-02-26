cmake_minimum_required(VERSION 2.6)

project(CartesianMeshRuntime CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)
find_package(MPI REQUIRED)

add_definitions(-DCMR_ENABLE_COLOR)

enable_testing()

set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
include_directories(${MPI_INCLUDE_PATH})

add_executable(cartesianmeshruntime main.cpp)
target_link_libraries(cartesianmeshruntime cmr ${MPI_LIBRARY})

add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(texparsor)