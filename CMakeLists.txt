######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#Cmake basic options
cmake_minimum_required(VERSION 2.6)
project(CartesianMeshRuntime CXX)
set(CMR_VERSION "0.0.0-dev")

######################################################
#global setup
enable_testing()
add_definitions(-DVERSION="${CMR_VERSION}")
add_definitions(-DCMR_ENABLE_COLOR)

######################################################
#Add internal search path for scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

######################################################
#Delegate some cmake scripts
include(${CMAKE_SOURCE_DIR}/cmake/user-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cpack-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/check-deps.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/enable-options.cmake)

######################################################
#some include dirs
include_directories(${MPI_INCLUDE_PATH})

######################################################
#manage subdirs
add_subdirectory(lib)
add_subdirectory(texparsor)
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)

######################################################
#build cmr exe (for tests)
add_executable(cartesianmeshruntime main.cpp)
target_link_libraries(cartesianmeshruntime cmr ${MPI_LIBRARY})

######################################################
#INSTALL(TARGETS htopmld
#        RUNTIME DESTINATION bin)
