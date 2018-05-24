#!/bin/sh
echo "running example ${1}"

./build/main.exe \
    --p "./examples/${1}/p.txt" \
    --q "./examples/${1}/q.txt" \
    --r "./examples/${1}/r.txt" \
    -o "./examples/${1}/results/"

echo %errorlevel%

python plot_pqr.py \
    --p "./examples/${1}/p.txt" \
    --q "./examples/${1}/q.txt" \
    --r "./examples/${1}/r.txt" \
    -o "./examples/${1}/results/" & echo "open plot_pqr.py"

python plot3d_with_holes.py "./examples/${1}/results/R_*.txt" & echo "open plot3d_with_holes.py"