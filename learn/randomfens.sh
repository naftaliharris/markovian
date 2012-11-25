#!/bin/bash

python random_positions.py $1 $2 | pgn-extract --notags --noresults -l/dev/null -C -D -F -N -V -w1000000000 | grep "{.*}" | sed 's/.*{ "//' | sed 's/" }//'
