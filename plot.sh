#!/bin/bash

data=build/out.p

gnuplot -persist <<-EOFMarker
    plot '$data' with filledcurves closed
EOFMarker