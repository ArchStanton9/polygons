#!/usr/bin/env bash

BASE_DIR="../build/grid"

data="$BASE_DIR/result.tr"

gnuplot -persist <<-EOFMarker
    plot '$data' with lines
EOFMarker