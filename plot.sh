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

PKG_OK=$(dpkg-query -W --showformat='${Status}\n' gnuplot)

if [ "" == "$PKG_OK" ]; then
  echo "gnuplot is not found"
  sudo apt-get --force-yes --yes install gnuplot
  echo "gnuplot is installed"
fi

gnuplot -persist <<-EOFMarker
    plot '$file' with filledcurves closed
EOFMarker