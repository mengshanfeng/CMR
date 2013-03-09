add_library(cmr common/CMRSpaceSplitter.cpp common/CMRGeometry.cpp common/CMRDebug.cpp common/CMRCommon.cpp
	domain/CMRAbstractDomain.cpp domain/CMRComposedDomain.cpp domain/CMRDomainStorage.cpp domain/CMRVarSystem.cpp
	communication/CMRBufferedComm.cpp communication/CMRMPIComm.cpp communication/CMRMPICommFactory.cpp communication/CMRComm.cpp communication/CMRCommSchem.cpp communication/CMRCommFactory.cpp )
target_link_libraries(cmr ${MPI_LIBRARY})
