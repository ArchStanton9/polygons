#!/usr/bin/env bash

BASE_DIR="../build/minkowski"

a="$BASE_DIR/polyA.p"
b="$BASE_DIR/polyB.p"
sum="$BASE_DIR/sum.p"
diff="$BASE_DIR/diff.p"

gnuplot -persist <<-EOFMarker
    set xzeroaxis solid 1 linewidth 2.5
    set yzeroaxis solid 1 linewidth 2.5
    set style fill transparent solid
    set ytics 1
    set xtics 1
    set grid
    plot \
    '$diff' title 'diff' with filledcurves closed ,\
    '$a' title 'a' with filledcurves closed ,\
    '$b' title 'b' with filledcurves closed ,\
    '$sum' title 'sum' with filledcurves closed
    
EOFMarker