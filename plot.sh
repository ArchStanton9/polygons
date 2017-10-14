#!/bin/bash

file="$1"

if [ "" == "$file" ]; then
    echo "Argument error. Input file is not specified"
    exit 1
fi

if [ -e "$file" ]; then
    echo "Plotting $file"
else
    echo "File '$file' is not found"
    exit 1
fi

gnuplot -persist <<-EOFMarker
    plot '$file' with filledcurves closed
EOFMarker