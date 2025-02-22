#!/bin/bash -
set -e

candidates=( $(find ./src/ -name "*.c" | sed -E "s/\.\/src\/([^\.]*).c/\1/g") )

if [ ! -x "/usr/bin/time" ]
then
    >&2 echo "Missing /usr/bin/time for benchmarking"
    exit 2
fi

declare -A results
declare -a programs
for prog in ${candidates[@]}
do
    if [ ! -x "${prog}" ]
    then
        >&2 echo "Did not find executable ${prog}"
    else
        programs+=("${prog}")
    fi
done

if [ "${#candidates[@]}" != "${#programs[@]}" ]
then
    >&2 echo "Did not find all executables, have you ran make?"
fi

declare -a all_results

if [ -t 1 ]
then
    echo "Detected algorithms: ${programs[@]}"
else
    echo "algorithm; N; runtime_secs; memory_usage_kB"
fi

for exp in {5..9}
do
    N=$(( 10**exp ))

    for prog in ${programs[@]}
    do
        if [ -t 1 ]
        then
            echo -n "Running ${prog} N=$N..."
        fi
        result=( $( ( /usr/bin/time -f "%U %M" ./${prog} $N 1> /dev/null ) 2>&1 ) )
        results["${prog}"]="${result[@]}"

        if [ -t 1 ]
        then
            echo "OK; ${result[0]} seconds (${result[1]} kB)"
        else
            echo "${prog}; $N; ${result[0]}; ${result[1]}"
        fi
    done
done
