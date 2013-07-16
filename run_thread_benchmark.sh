#!/usr/bin/env bash
EXPECTED_ARGS=2
E_BADARGS=65
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` {arg} {num_points}"
  exit $E_BADARGS
fi

count=$2
csv_file="mc_$1_$count.csv"
program="bench"
rm -f $csv_file
for i in {1..100}
do
	echo    ""  >> $csv_file
	echo -n  $i >> $csv_file
	echo -n "," >> $csv_file
	./$program $1 $count $i >> $csv_file
done
