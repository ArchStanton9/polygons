#!/usr/bin/env bash

BASE_DIR="../../cmake-build-debug"

# join all files starts with 'c'
DATA="<(cat $BASE_DIR/c*.p)"

gnuplot -persist <<-EOFMarker
    set style fill transparent solid 0.5 noborder
    plot '$DATA' \
    title 'intersection' \
    with filledcurves closed
EOFMarker