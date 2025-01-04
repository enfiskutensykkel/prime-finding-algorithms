#!/bin/bash -
set -e

programs=(b001 trial sieve dijkstra)

if [ ! -x "/usr/bin/time" ]
then
    >&2 echo "Missing /usr/bin/time for benchmarking"
    exit 2
fi

N=100000000

for prog in ${programs[@]}
do
    if [ ! -x "${prog}" ]
    then
        >&2 echo "Missing executable ${prog}. Have you ran make?"
        exit 3
    fi
done


echo "Running programs with N=$N..."
for prog in ${programs[@]}
do
    benchmark=( $( ( /usr/bin/time -f "%U %M" ./${prog} $N 1> /dev/null ) 2>&1 ) )
    echo "algorithm=${prog}, iterations=$N, runtime=${benchmark[0]}secs, memory_usage=${benchmark[1]}kB"
done
echo "Done"
