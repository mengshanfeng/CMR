#!/bin/bash
######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
PROJECT_FILE="$1"
shift 1
GEN_FILTERS="$*"

######################################################
#Find install dir to get access to lib scripts and templates
PREFIX="$(dirname "$0")/.."

######################################################
#clean prefix
FILE_PREFIX="$(cd "${PREFIX}" && pwd)"

######################################################
TEMPLATES_DIR="${PREFIX}/share/cmr/templates/"
EXAMPLE_DIR="${PREFIX}/share/cmr/examples/"
BIN_DIR="${PREFIX}/bin"
FILE_PREFIX=$(basename ${PROJECT_FILE} | sed -e 's/\.cmr.xml//g')

######################################################
#enable exit on error
set -e
#set -x

######################################################
#check args
if [ -z "${PROJECT_FILE}" ]; then
	echo "Invalid argument." 1>&2
	echo "Usage : $0 {file.cmr.xml} [list_of_files_to_generate]" 1>&2
	echo "" 1>&2
	echo "You can found some example of file format in ${EXAMPLE_DIR}"
	exit 1
fi

######################################################
#Check the XML format
#xmllint --schema "${TEMPLATES_DIR}/htopml-project.xsd" "${PROJECT_FILE}" --noout || exit 1

######################################################
#Register generated file for distclean
#Params : {fname}
function reg_gen_file()
{
	if [ ! -e "./manifest-generated.txt" ]; then
		echo "manifest-generated.txt" > "./manifest-generated.txt"
	fi
	
	if [ -z "$(grep "^$1$" './manifest-generated.txt')" ]; then
		echo "$1" >> "./manifest-generated.txt"
	fi
}

######################################################
#Params : {filename}
function check_if_want_to_gen()
{
	#check in params
	if [ -z "${GEN_FILTERS}" ]; then
		return 0
	else
		case " ${GEN_FILTERS} " in
			*" $1 "*)
				return 0;
				;;
			*)
				return 1;
				;;
		esac
	fi
}

######################################################
#Generate all files from templates
function gen_from_template()
{
	for tmp in "${TEMPLATES_DIR}/"*.template
	do
		#calc names
		outfnamebase=$(basename "$tmp" | sed -e 's/\.template$//g')
		xslfile=$(mktemp)

		#add prefix
		outfname="${FILE_PREFIX}-${outfnamebase}"

		#Check if want to gen
		if check_if_want_to_gen "${outfnamebase}"
		then
			#mark as generated
			reg_gen_file "${outfname}"
			#gen and display
			echo "${outfname}"
			echo "  + Generate ${xslfile}"
			"${BIN_DIR}/cmr-template2xslt" "${tmp}" > "${xslfile}"
			echo "  + Generate ${outfname}"
			xsltproc -o "${outfname}" "${xslfile}" "${PROJECT_FILE}"
			sed -i "s/{{FILE_PREFIX}}/${FILE_PREFIX}/g" "${outfname}"
			echo "  + Remove temporaty ${xslfile}"
			rm -f "${xslfile}"
		else
			echo "${outfname} [SKIPED]"
		fi
	done
}

######################################################
function check_gen()
{
	local fname="$1"

	if check_if_want_to_gen "$fname"; then
		echo " + Generate ${fname}"
		reg_gen_file "${fname}"
		return 0;
	else
		return 1;
	fi
}

######################################################
function setup_makefile()
{
	check_gen 'Makefile' || return 0
	sed -e "s#{{CMR_PREFIX}}#${PREFIX}#g" -e "s#{{PROJECT_FILE}}#${PROJECT_FILE}#g" -e "s#{{FILE_PREFIX}}#${FILE_PREFIX}#g" "${TEMPLATES_DIR}/Makefile.in" > Makefile
}

######################################################
function gen_source_code()
{
	check_gen "${FILE_PREFIX}.cpp" || return 0
	"${PREFIX}/bin/cmrcompiler" "$PROJECT_FILE" > "${FILE_PREFIX}.cpp"
}

######################################################
function gen_manifest()
{
	echo " + Generate manifest-generated.txt"
	reg_gen_file 'manifest-generated.txt'
}

######################################################
function copy_files_if_not_local()
{
	path=$(dirname "$PROJECT_FILE")
	for tmp in $path/*; do
		name=$(basename $tmp)
		if [ -e "$tmp" ] && [ ! -e "$name" ]; then
			echo " + Copy $name"
			cp "$tmp" .
		fi
	done
}

######################################################
#main
echo "Generating code..."
setup_makefile
gen_source_code
gen_from_template
copy_files_if_not_local
gen_manifest
if [ -f "${FILE_PREFIX}-gen-animate-gif.sh" ]; then
	chmod a+x "${FILE_PREFIX}-gen-animate-gif.sh"
fi
echo "End of code generation."
