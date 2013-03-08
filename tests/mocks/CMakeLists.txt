#setup directories
include_directories(${UNITTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib)

add_library(cmr_mocks MockCommunicator.cpp MockAbstractDomain.cpp )
target_link_libraries(cmr_mocks cmr)

add_library(cmr_skip_mpi_init MockMpiInit.cpp) 