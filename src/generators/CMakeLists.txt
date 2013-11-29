######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
INSTALL(PROGRAMS cmr-generate.sh
        DESTINATION bin RENAME cmr-generate)

######################################################
INSTALL(FILES Makefile.in cmr2html.xsl cmr.dtd
		DESTINATION share/cmr/templates/)
