#!/bin/sh
CURRDIR=$(pwd)
BUILD_DIR=$CURRDIR/../build
MICRO=$BUILD_DIR/Micro
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
        diff -b -B $f ../Output/$f 
done
cd ..
