######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(cmr_tex_parsor CMRLatexFormula.cpp CMRLatexException.cpp CMRLatexParsorContext.cpp CMREntityMatching.cpp CMREqDepMatrix.cpp CMRTexParsor.cpp CMRLatexEntity.cpp CMRParsorBasics.cpp CMRLatexEntity2.cpp)

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
