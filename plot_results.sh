#!/usr/bin/env bash
TEMP="$(mktemp)"
rm -f $TEMP
echo "set terminal pngcairo" >> $TEMP
echo "set output \"graph.png\"" >> $TEMP
echo "set datafile separator ',' ">> $TEMP
echo -n "plot " >> $TEMP
i=0
for file in $(ls mc*.csv); do
	if [ "$i" -gt 0 ]; then
		echo -n ",">>$TEMP
	fi
	echo -n "\"$file\" with linespoints" >> $TEMP
	i=`expr $i + 1`
done
echo "" >> $TEMP
gnuplot $TEMP
rm $TEMP
