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
include_directories(${UNITTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
add_definitions(-DUNITTEST)

######################################################
#test list
set(ALLOC_TEST_LIST TestAbstractDomain TestDomainStorage TestRect TestVect2D)

######################################################
#Now you can build your tests
foreach(i ${ALLOC_TEST_LIST})
	add_executable(${i} ${i}.cpp)
	target_link_libraries(${i} ${UNITTEST_LIBRARY} cmr cmr_mocks  ${MPI_LIBRARY})
	cmr_add_test(${i})
endforeach(i)

######################################################
add_subdirectory(mocks)

######################################################
add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox cmr cmr_mocks  ${MPI_LIBRARY})

######################################################
add_executable(lbm lbm_test.cpp)
target_link_libraries(lbm cmr cmr_mocks  ${MPI_LIBRARY})