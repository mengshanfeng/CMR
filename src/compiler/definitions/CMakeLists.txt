######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(cmr_tex_definitions STATIC CMRProjectEntity.cpp CMRProjectContext.cpp CMRGenCode.cpp CMRProjectMeshVar.cpp CMRProjectIterator.cpp CMRProjectConstant.cpp CMRProjectCode.cpp CMRProjectDefinition.cpp CMRProjectAction.cpp CMRProjectAlias.cpp)
target_link_libraries(cmr_tex_definitions cmr_tex_parsor cmr_tex_common)

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
