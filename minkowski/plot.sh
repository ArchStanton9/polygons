#!/usr/bin/env bash

BASE_DIR="../build/minkowski"

a="$BASE_DIR/polyA.p"
b="$BASE_DIR/polyB.p"
sum="$BASE_DIR/sum.p"

gnuplot -persist <<-EOFMarker
    set style fill transparent solid 0.5 noborder
    plot \
    '$a' title 'a' with filledcurves closed ,\
    '$b' title 'b' with filledcurves closed ,\
    '$sum' title 'sum' with filledcurves closed
EOFMarker