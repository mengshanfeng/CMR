######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox cmr  ${CMR_MPI_CXX_LIBRARIES})

######################################################
add_executable(lbm lbm_test.cpp)
target_link_libraries(lbm cmr  ${CMR_MPI_CXX_LIBRARIES})
