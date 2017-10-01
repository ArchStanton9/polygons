#!/usr/bin/env bash

BASE_DIR="../cmake-build-debug/intersection"

a="$BASE_DIR/a.p"
b="$BASE_DIR/b.p"

gnuplot -persist <<-EOFMarker
    set style fill transparent solid 0.5 noborder
    plot \
    '$a' title 'a' with filledcurves closed ,\
    '$b' title 'b' with filledcurves closed
EOFMarker
