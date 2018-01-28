#!/usr/bin/env bash

BASE_DIR="../build/game1"

P="$BASE_DIR/P.p"
Q="$BASE_DIR/Q.p"
M="$BASE_DIR/M.p"
W="$BASE_DIR/W.p"

gnuplot -persist <<-EOFMarker
    set xzeroaxis solid 1 linewidth 2.5
    set yzeroaxis solid 1 linewidth 2.5
    set style fill transparent solid 0.5
    plot \
    '$P' title 'P' with filledcurves closed ,\
    '$Q' title 'Q' with filledcurves closed ,\
    '$M' title 'M' with filledcurves closed 

EOFMarker