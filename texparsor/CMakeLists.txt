
add_executable(texparsor CMRProjectConstant.cpp CMRProjectEntity.cpp main.cpp)
target_link_libraries(texparsor cmr_tex_parsor)

add_subdirectory(parsor)