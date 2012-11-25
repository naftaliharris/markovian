#!/bin/bash

pgn-extract -oallgames -C -D -e/usr/share/pgn-extract/eco.pgn --nomovenumbers -N -ttag -V -w100000000 -Wlalg -ffile_list
