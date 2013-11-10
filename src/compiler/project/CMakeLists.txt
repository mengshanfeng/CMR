######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(cmr_tex_project STATIC CMRProjectXMLLoader.cpp CMRXmlDoc.cpp CMRProject.cpp)
target_link_libraries(cmr_tex_project cmr_tex_parsor cmr_tex_common)

######################################################
add_subdirectory(tests)