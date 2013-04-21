######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
include(${CMAKE_SOURCE_DIR}/cmake/unittest-macros.cmake)

######################################################
#add_definitions(-DTEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/datas")

######################################################
#setup directories
include_directories(${UNITTEST_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_definitions(-DUNITTEST)

######################################################
#test list
set(ALLOC_TEST_LIST TestProjectEntity TestProjectContext TestGenCode TestProjectMeshVar)

######################################################
#Now you can build your tests
foreach(i ${ALLOC_TEST_LIST})
	add_executable(${i} ${i}.cpp)
	target_link_libraries(${i} ${UNITTEST_LIBRARY} cmr_tex_project ${MPI_CXX_LIBRARIES})
	cmr_add_test(${i})
endforeach(i)

