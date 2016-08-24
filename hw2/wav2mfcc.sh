#!/bin/bash
#David Shagam
#Spoken Lang Processing
#Homework 2
if [ $# -lt 2 ]
then
echo "insufficient arguments given $# args"
else
HList -i 100 -C $1 $2 | ./postproc
fi