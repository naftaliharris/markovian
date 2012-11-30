#!/bin/bash

usage()
{
    echo "Usage: $0 [pgnfile] [number of fens]"
}


if [ $# -ne "2" ]
then
    usage
    exit -1
fi

python random_positions.py $1 $2 | pgn-extract --notags --noresults -l/dev/null -C -D -F -N -V -w1000000000 | grep "{.*}" | sed 's/.*{ "//' | sed 's/" }//'
