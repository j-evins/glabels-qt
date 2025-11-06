#! /bin/sh

inkscape -f ../icons/glabels-flat/16x16/apps/glabels.svg    -w  16 -h  16 -e /tmp/glabels-016x016x32.png
inkscape -f ../icons/glabels-flat/32x32/apps/glabels.svg    -w  32 -h  32 -e /tmp/glabels-032x032x32.png
inkscape -f ../icons/glabels-flat/48x48/apps/glabels.svg    -w  48 -h  48 -e /tmp/glabels-048x048x32.png
inkscape -f ../icons/glabels-flat/scalable/apps/glabels.svg -w 256 -h 256 -e /tmp/glabels-256x256x32.png

convert /tmp/glabels-016x016x32.png -colors 256 -depth 8 /tmp/glabels-016x016x08.png
convert /tmp/glabels-032x032x32.png -colors 256 -depth 8 /tmp/glabels-032x032x08.png
convert /tmp/glabels-048x048x32.png -colors 256 -depth 8 /tmp/glabels-048x048x08.png

icotool -c -o glabels.ico \
	/tmp/glabels-048x048x08.png \
	/tmp/glabels-032x032x08.png \
	/tmp/glabels-016x016x08.png \
	/tmp/glabels-256x256x32.png \
	/tmp/glabels-048x048x32.png \
	/tmp/glabels-032x032x32.png \
	/tmp/glabels-016x016x32.png
