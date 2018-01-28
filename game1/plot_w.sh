#!/usr/bin/env bash

BASE_DIR="../build/game1"

M="$BASE_DIR/M.p"

# join all holes
#W="<(cat $BASE_DIR/W*.p)"

W0="$BASE_DIR/W0.p"
W1="$BASE_DIR/W1.p"
W2="$BASE_DIR/W2.p"
W3="$BASE_DIR/W3.p"
W4="$BASE_DIR/W4.p"

gnuplot -persist <<-EOFMarker
    set xzeroaxis solid 1 linewidth 2.5
    set yzeroaxis solid 1 linewidth 2.5
    set style fill transparent solid 0.5
    plot \
    '$M' title 'M' with filledcurves closed ,\
    '$W0' title 'W0' with filledcurves closed ,\
    '$W1' title 'W1' with filledcurves closed ,\
    '$W2' title 'W2' with filledcurves closed ,\
    '$W3' title 'W3' with filledcurves closed ,\
    '$W4' title 'W4' with filledcurves closed ,\

EOFMarker