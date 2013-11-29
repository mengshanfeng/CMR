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
INSTALL(PROGRAMS cmr-template2xslt.sh
        DESTINATION bin RENAME cmr-template2xslt)

######################################################
INSTALL(FILES Makefile.in cmr2html.xsl cmr.dtd cmr-raw-output.h.template cmr-raw-reader.cpp.template gen-animate-gif.sh.template
		DESTINATION share/cmr/templates/)
