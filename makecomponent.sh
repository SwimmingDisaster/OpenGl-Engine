#!/bin/sh

cp other/components/base.h include/code/assets/$1.h
cp other/components/base.cpp src/code/assets/$1.cpp

sed -i s/COMPONENTNAME/$2/g include/code/assets/$1.h 
sed -i s/COMPONENTNAME/$2/g src/code/assets/$1.cpp
sed -i s/ABCDEFG/$1/g src/code/assets/$1.cpp
