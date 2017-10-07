#!/usr/bin/env bash

BASE_DIR="../cmake-build-debug/noise"

file="$BASE_DIR/input.p"

gnuplot -persist <<-EOFMarker
    set style fill transparent solid 0.5 noborder
    plot \
    '$file' title 'a' with filledcurves closed
EOFMarker