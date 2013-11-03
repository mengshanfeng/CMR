######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})
 
######################################################
add_subdirectory(parsor)
add_subdirectory(project)
add_subdirectory(definitions)
add_subdirectory(transformations)
add_subdirectory(old_code)

######################################################
add_executable(texparsor_new main.cpp)
target_link_libraries(texparsor_new cmr_tex_project cmr_tex_parsor cmr_tex_definitions cmr_tex_transf)
