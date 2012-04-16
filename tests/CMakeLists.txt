
find_package(svUnitTest)

if(SVUNITTEST_FOUND)
	set(UNITTEST_INCLUDE_DIR ${SVUNITTEST_INCLUDE_DIR})
	set(UNITTEST_LIBRARY ${SVUNITTEST_LIBRARY})
else(SVUNITTEST_FOUND)
	set(UNITTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/svUnitTest_fake/include)
	set(UNITTEST_LIBRARY)
endif(SVUNITTEST_FOUND)

#setup directories
include_directories(${UNITTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

#test list
set(ALLOC_TEST_LIST TestAbstractDomain)

#Now you can build your tests
foreach(i ${ALLOC_TEST_LIST})
	add_executable(${i} ${i}.cpp)
	target_link_libraries(${i} ${UNITTEST_LIBRARY} cmr cmr_mocks)
	add_test(${i} ${i})
endforeach(i)

add_subdirectory(mocks)