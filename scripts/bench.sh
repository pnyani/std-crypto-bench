#!/bin/sh
set -e

BENCH="${BENCH:-./bin/bench}"
SCRIPTS_DIR="$(dirname "$0")"
PARSE="${SCRIPTS_DIR}/parse_ipb.py"
OUT_DIR="${OUT_DIR:-.}"

ALGOS="AES-128 AES-256 ARIA-128 SEED DES 3DES-2KEY 3DES-3KEY"
SIZES="1 10"
DIRS="enc dec"
REPEAT=10

if [ ! -x "$BENCH" ]; then
    echo "error: '$BENCH' not found. run 'make' first." >&2
    exit 1
fi

printf "%-12s  %12s  %4s  %20s  %10s\n" "Algorithm" "Bytes" "Dir" "Ir" "IPB"
printf '%s\n' "--------------------------------------------------------------------"

for size in $SIZES; do
    data_bytes=$(( size * 1024 * 1024 ))
    for algo in $ALGOS; do
        for dir in $DIRS; do
            outfile="${OUT_DIR}/callgrind_${algo}_${size}MB_${dir}.out"

            valgrind \
                --tool=callgrind \
                --cache-sim=no   \
                --branch-sim=no  \
                --callgrind-out-file="$outfile" \
                --instr-atstart=no \
                --quiet \
                "$BENCH" "$algo" "$size" "$dir"

            python3 "$PARSE" "$outfile" "$algo" "$data_bytes" "$dir" "$REPEAT"
        done
    done
done
