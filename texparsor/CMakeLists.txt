
add_executable(texparsor CMRProjectConstant.cpp CMRProjectEntity.cpp CMRProjectEquation.cpp CMRProjectIterator.cpp CMRProjectVariable.cpp CMRProjectAction.cpp CMRProject.cpp main.cpp)
target_link_libraries(texparsor cmr_tex_parsor)

add_subdirectory(parsor)