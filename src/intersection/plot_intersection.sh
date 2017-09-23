#!/usr/bin/env bash

a="../../cmake-build-debug/a.p"
b="../../cmake-build-debug/b.p"

gnuplot -persist <<-EOFMarker
    set style fill transparent solid 0.5 noborder
    plot \
    '$a' title 'a' with filledcurves closed ,\
    '$b' title 'b' with filledcurves closed
EOFMarker