#!/usr/bin/env bash

BASE_DIR="../build/difference"

a="$BASE_DIR/a.p"
b="$BASE_DIR/b.p"

# join all holes
diff="<(cat $BASE_DIR/h*.p)"

gnuplot -persist <<-EOFMarker
    set xzeroaxis solid 1 linewidth 2.5
    set yzeroaxis solid 1 linewidth 2.5
    set style fill transparent solid 0.5
    plot \
    '$a' title 'a' with filledcurves closed ,\
    '$b' title 'b' with filledcurves closed ,\
    '$diff' title 'diff' with filledcurves closed

EOFMarker