######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(cmr_tex_definitions STATIC ProjectEntity.cpp ProjectContext.cpp GenCode.cpp ProjectMeshVar.cpp ProjectIterator.cpp ProjectConstant.cpp CMRProjectCode.cpp ProjectDefinition.cpp ProjectAction.cpp ProjectMathAlias.cpp ProjectCallAction.cpp ProjectOutput.cpp ProjectEntityNew.cpp)
target_link_libraries(cmr_tex_definitions cmr_tex_parsor cmr_tex_common)

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
