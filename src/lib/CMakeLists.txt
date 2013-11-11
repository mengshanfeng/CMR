######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
cmr_setup_mpi()

######################################################
include_directories(${CMR_MPI_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

######################################################
add_library(cmr common/CMRSpaceSplitter.cpp common/CMRGeometry.cpp common/CMRDebug.cpp common/CMRCommon.cpp
	domain/CMRAbstractDomain.cpp domain/CMRComposedDomain.cpp domain/CMRDomainStorage.cpp domain/CMRVarSystem.cpp 
	communication/CMRBufferedComm.cpp communication/CMRComm.cpp communication/CMRCommSchem.cpp communication/CMRCommFactory.cpp 
	mpi/CMRMPIDomainBuilder.cpp mpi/CMRMPIComm.cpp mpi/CMRMPICommFactory.cpp mpi/CMRMPIReduction.cpp runner/CMRBasicOutputer.cpp)
target_link_libraries(cmr ${CMR_MPI_CXX_LIBRARIES})

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
