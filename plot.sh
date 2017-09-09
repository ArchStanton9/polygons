#!/bin/bash

gnuplot -persist <<-EOFMarker
    plot 'build/out.p' with filledcurves closed
EOFMarker