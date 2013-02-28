
find_package(svUnitTest)

if(SVUNITTEST_FOUND)
	set(UNITTEST_INCLUDE_DIR ${SVUNITTEST_INCLUDE_DIR}/svUnitTest)
	set(UNITTEST_LIBRARY ${SVUNITTEST_LIBRARY})
else(SVUNITTEST_FOUND)
	set(UNITTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/svUnitTest_fake/include)
	set(UNITTEST_LIBRARY)
endif(SVUNITTEST_FOUND)

#setup directories
include_directories(${UNITTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
add_definitions(-DUNITTEST)

#test list
set(ALLOC_TEST_LIST TestAbstractDomain TestDomainStorage TestRect TestVect2D)

#Now you can build your tests
foreach(i ${ALLOC_TEST_LIST})
	add_executable(${i} ${i}.cpp)
	target_link_libraries(${i} ${UNITTEST_LIBRARY} cmr cmr_mocks  ${MPI_LIBRARY})
	add_test(${i} ${i})
endforeach(i)

add_subdirectory(mocks)

add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox cmr cmr_mocks  ${MPI_LIBRARY})

add_executable(lbm lbm_test.cpp)
target_link_libraries(lbm cmr cmr_mocks  ${MPI_LIBRARY})