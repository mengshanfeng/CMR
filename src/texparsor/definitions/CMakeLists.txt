######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(cmr_tex_project CMRProjectEntity.cpp CMRProjectContext.cpp CMRGenCode.cpp CMRProjectMeshVar.cpp CMRProjectIterator.cpp)
target_link_libraries(cmr_tex_project cmr_tex_parsor)

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
