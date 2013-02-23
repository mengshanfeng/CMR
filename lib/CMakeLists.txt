add_library(cmr common/CMRSpaceSplitter.cpp common/CMRGeometry.cpp common/CMRDebug.cpp common/CMRCommon.cpp
	domain/CMRComposedDomain.cpp domain/CMRDomainStorage.cpp domain/CMRAbstractDomain.cpp
	communication/CMRBufferedComm.cpp communication/CMRMPIComm.cpp communication/CMRMPICommFactory.cpp communication/CMRComm.cpp communication/CMRCommSchem.cpp communication/CMRCommFactory.cpp )
