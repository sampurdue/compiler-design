#!/bin/sh
CURRDIR=$(pwd)
MICRO=$CURRDIR/../Micro
rm -rf Output
mkdir Output
cd TestFiles
for f in *; do
	outputf=$f
	$MICRO $f > $CURRDIR/Output/${outputf%.*}.out
done 
cd ..

cd TestOutput
for f in *; do
        diff $f ../Output/$f 
done
cd ..
