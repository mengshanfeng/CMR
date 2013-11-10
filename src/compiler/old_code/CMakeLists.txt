######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
include_directories(../)

######################################################
add_executable(texparsor CMRProjectConstant.cpp CMRProjectEntity.cpp CMRProjectEquation.cpp CMRProjectIterator.cpp CMRProjectVariable.cpp CMRProjectAction.cpp CMRProject.cpp main.cpp CMRCodeTree.cpp CMRProjectDefinition.cpp CMRProjectContext.cpp CMRProjectTransformation.cpp CMRGenCode.cpp)
target_link_libraries(texparsor cmr_tex_parsor)
