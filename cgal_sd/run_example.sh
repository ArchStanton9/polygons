#!/bin/sh
echo "running example ${1}"

./build/main.exe \
    --p "./examples/${1}/p.txt" \
    --q "./examples/${1}/q.txt" \
    --r "./examples/${1}/r.txt" \
    -o "./examples/${1}/results/"

OUT=$?
echo "Exit code: ${OUT}"

if [ $OUT -eq 0 ];then
   echo "running plots!"
   python plot_pqr.py \
    --p "./examples/${1}/p.txt" \
    --q "./examples/${1}/q.txt" \
    --r "./examples/${1}/r.txt" \
    -o "./examples/${1}/results/" & echo "run plot_pqr.py"

    python plot3d_set.py "./examples/${1}/results/R_*.txt" & echo "run plot3d_set.py"
fi