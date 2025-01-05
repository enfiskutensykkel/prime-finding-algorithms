#!/bin/bash -
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

if [ ${#prog[@]} -eq 0 ]
then
    >&2 echo "Could not find any programs"
    exit 2
fi

echo "Found programs: ${programs[@]}"

echo -n "Generating primes up to $N..."
h=$(/usr/bin/env python genprimes.py $N | sha256sum)
echo "OK"

echo

declare -A results
for prog in ${programs[@]}
do
    echo -n "Testing ${prog}..."
    result=$(./${prog} $N 2> /dev/null | sha256sum)
    if [ $? -eq 0 ] && [ "${result}" == "${h}" ]
    then
        results["${prog}"]=0
        echo "OK"
    else
        if [ $? -ne 0 ]
        then
            results["${prog}"]=1
        elif [ "${result}" != "${h}" ]
        then
            results["${prog}"]=2
        else
            results["${prog}"]=255
        fi
        echo "FAIL"
    fi
done

status_code=0
for prog in ${!results[@]}
do
    result=${results["${prog}"]}
    case "$result" in
        "0")
            ;;
        "1")
            >&2 echo "${prog} terminated with non-zero error code"
            ;;
        "2")
            >&2 echo "${prog} is not generating correct output"
            status_code=1
            ;;
        *)
            >&2 echo "Unexpected error with ${prog}"
            status_code=1
            ;;
    esac
done

exit ${status_code}
