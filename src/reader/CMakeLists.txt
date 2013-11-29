include_directories(../lib/)
add_library(cmrrawreader CMRRawReader.cpp)

######################################################
#install procedure
INSTALL(TARGETS cmrrawreader
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
        LIBRARY DESTINATION lib${LIB_SUFFIX})
INSTALL(FILES CMRRawReader.h DESTINATION include/cmr/reader)
