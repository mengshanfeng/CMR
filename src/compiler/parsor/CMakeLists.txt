######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(cmr_tex_parsor LatexFormula.cpp LatexException.cpp LatexParsorContext.cpp EntityMatching.cpp EqDepMatrix.cpp TexParsor.cpp LatexEntityOld.cpp ParsorBasics.cpp LatexEntity.cpp)

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
