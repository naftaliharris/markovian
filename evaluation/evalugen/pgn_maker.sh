#!/bin/bash

pgn-extract -oallgames -C -D -e/usr/share/pgn-extract/eco.pgn --nomovenumbers -N -ttag -V -w100000000 -Wlalg -ffile_list
#pgn-extract -oallgames -C -D -e/usr/share/pgn-extract/eco.pgn --nomovenumbers -N -ttag -V -w100000000 -Wlalg *.pgn
#pgn-extract -oallgames -C -D -e/usr/share/pgn-extract/eco.pgn --nomovenumbers -N -V -w100000000 -Wlalg -llogfile *.pgn 
#pgn-extract -oallgames -bl10 -C -D -e/usr/share/pgn-extract/eco.pgn --nomovenumbers -N -ttags -V -w100000000 -Wlalg *.pgn
