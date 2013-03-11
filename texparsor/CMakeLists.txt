add_library(cmr_tex_parsor CMREntityMatching.cpp CMREqDepMatrix.cpp CMRTexParsor.cpp CMRLatexEntity.cpp CMRParsorBasics.cpp)

add_executable(texparsor main.cpp)
target_link_libraries(texparsor cmr_tex_parsor)