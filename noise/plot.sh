#!/usr/bin/env bash

BASE_DIR="../build/simplify/debug"

a="$BASE_DIR/input.p"
b="$BASE_DIR/output.p"

gnuplot -persist <<-EOFMarker
    set style fill transparent solid 0.5 noborder
    plot \
    '$a' title 'a' with filledcurves closed ,\
    '$b' title 'b' with filledcurves closed
EOFMarker
