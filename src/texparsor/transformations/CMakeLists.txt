######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(cmr_tex_transf CMRTransformationExtractLoops.cpp CMRTransformationExpandExpo.cpp CMRTransformationReplace.cpp CMRTransformation.cpp CMRTransformationBasic.cpp CMRTransformationMarkNoTransf.cpp CMRTransformationExpandFrac.cpp CMRTransformationImplicitMul.cpp)
target_link_libraries(cmr_tex_transf cmr_tex_project)

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
