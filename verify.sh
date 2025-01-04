#!/bin/bash -
set -e

programs=(b001 trial sieve dijkstra)

N=100000

for prog in ${programs[@]}
do
    if [ ! -x "${prog}" ]
    then
        >&2 echo "Missing executable ${prog}. Have you ran make?"
        exit 3
    fi
done

echo "Generating primes up to $N..."
h=$(/usr/bin/env python genprimes.py $N | sha256sum)
echo "Done"

echo "Testing programs..."
for prog in ${programs[@]}
do
    if [ "`./${prog} $N | sha256sum`" != "${h}" ]
    then
        echo "${prog} is not generating correct output"
    else
        echo "${prog} is generating correct output"
    fi
done
echo "Done"
