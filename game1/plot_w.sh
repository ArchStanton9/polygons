#!/usr/bin/env bash

BASE_DIR="../build/game1"

M="$BASE_DIR/M.p"
W="$BASE_DIR/$1"

gnuplot -persist <<-EOFMarker
    set xzeroaxis solid 1 linewidth 2.5
    set yzeroaxis solid 1 linewidth 2.5
    set style fill transparent solid 0.5
    plot \
    '$M' title 'M' with filledcurves closed ,\
    '$W' title 'W' with filledcurves closed

EOFMarker