#!/bin/bash

#check args
if [ -z "$1" ] || [ -z "$2" ]; then
	echo "Usage : $0 {input.raw} {output.gif}" 1>&2
	exit 1
fi

#get values
INPUT_FILE=$1
OUTPUT_FILE=$2

#get info
DISPLAY_PATH=$(dirname "$0")
FRAMES=$("${DISPLAY_PATH}/display" --info ${INPUT_FILE} 0 | grep frames | sed -e 's/frames=//g')
IMAGE=$(grep obstacle_filename config.txt | grep -v '#' | sed -e 's/ //g' | sed -e 's/obstacle_filename=//g')
SCALE=$(grep obstacle_scale config.txt | grep -v '#' | sed -e 's/ //g' | sed -e 's/obstacle_scale=//g')
ROTATE=$(grep obstacle_rotate config.txt | grep -v '#' | sed -e 's/ //g' | sed -e 's/obstacle_rotate=//g')
WIDTH=$("${DISPLAY_PATH}/display" --info ${INPUT_FILE} 0 | grep width | sed -e 's/width=//g')

if [ ! -z "$IMAGE" ]; then
	out="tmp-transp.png"
	convert -fuzz 10%% -transparent '#FFFFFF' "$IMAGE" "$out"
	pos=$(echo "$WIDTH / 5 + 1" | bc)
	if [ -z "$SCALE" ] ;then SCALE=1; fi
	if [ -z "$ROTATE" ]; then ROTATE=0; fi
	img=", '$out' binary filetype=png origin=($pos,0,0) dx=$SCALE dy=$SCALE rotation=$ROTATE with rgbalpha notitle"
	echo $img
fi

#gen gnuplot command
gen_gnuplot_command()
{
	echo "set pm3d map"
	echo "set palette defined ( 0 '#000090', 1 '#000fff',2 '#0090ff',3 '#0fffee', 4 '#90ff70', 5 '#ffee00', 6 '#ff7000', 7 '#ee0000', 8 '#7f0000')"
	echo "set cbr [0:0.14]"
	echo "set term gif animate delay 5 size 1700,300"
	echo "set output '${OUTPUT_FILE}'"

	for tmp in $(seq 0 1 ${FRAMES})
	do
		echo "splot \"< ${DISPLAY_PATH}/display --gnuplot ${INPUT_FILE} $tmp\" u 1:2:4 ${img}"
	done
}

#call if
gen_gnuplot_command | gnuplot
