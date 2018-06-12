#!/bin/sh
echo "running example ${1}"

./build/main.exe \
    --p "./examples/${1}/p.txt" \
    --q "./examples/${1}/q.txt" \
    --m "./examples/${1}/m.txt" \
    -o "./examples/${1}/results/" --dt 0.2 --steps 50 --tr 0.001 --ks

    # --dt 0.2 --steps 50 --tr 0.0001



OUT=$?
echo "Exit code: ${OUT}"

if [ $OUT -eq 0 ];then
   echo "running plots!"
   python plot_pqm.py \
    --p "./examples/${1}/p.txt" \
    --q "./examples/${1}/q.txt" \
    --m "./examples/${1}/m.txt" \
    -o "./examples/${1}/results/" & echo "run plot_pqm.py"

    python plot3d_set.py "./examples/${1}/results/W_*.txt" & echo "run plot3d_set.py"
fi