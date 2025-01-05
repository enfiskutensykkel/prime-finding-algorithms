#!/bin/bash -
set -e

candidates=( $(find ./src/ -name "*.c" | sed -E "s/\.\/src\/([^\.]*).c/\1/g") )

N=100000

programs=()
for prog in ${candidates[@]}
do
    if [ ! -x "${prog}" ]
    then
        >&2 echo "Missing executable ${prog}"
    else
        programs+=("${prog}")
    fi
done

echo "Found programs: ${programs[@]}"

echo -n "Generating primes up to $N..."
h=$(/usr/bin/env python genprimes.py $N | sha256sum)
echo "OK"

echo

declare -A results
for prog in ${programs[@]}
do
    echo -n "Testing ${prog}..."
    result=$(./${prog} $N | sha256sum)
    if [ "${result}" != "${h}" ]
    then
        results["${prog}"]=0
        echo "FAIL"
    else
        results["${prog}"]=0
        echo "OK"
    fi
done

status_code=0
for prog in ${!results[@]}
do
    result=${results["${prog}"]}
    if [ "${result}" != "0" ]
    then
        status_code=1
        >&2 echo "${prog} is not generating correct output"
    fi
done

exit ${status_code}
