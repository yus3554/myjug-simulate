#!/bin/sh

cd `dirname $0`
make
./main slump.dat
gnuplot -e "
set terminal png;
set output 'slump.png';
plot 'slump.dat' with lines;
set terminal aqua;
set output;
exit;
"
echo 何かキーを押してください
read a
