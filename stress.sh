#!/usr/bin/env bash
# Stress test a solution against a brute force on random inputs.
#
#   ./stress.sh [solution.cpp] [brute.cpp] [gen.py] [iterations]
#   ./stress.sh                        # defaults: sol.cpp, stress/brute.cpp, stress/gen.py, 1000
#
# The solution is built WITH sanitizers, so this also catches UB and
# out-of-bounds access that happen to produce the right answer.

set -u

SOL=${1:-sol.cpp}
BRUTE=${2:-stress/brute.cpp}
GEN=${3:-stress/gen.py}
ITERS=${4:-1000}

CXX=${CXX:-g++}
STD="-std=c++23 -Ilib"
# no -DUSE_FILES: the stress harness pipes tests through stdin
DEBUG_FLAGS="$STD -O2 -g -D_UTSAV_ -D_GLIBCXX_DEBUG -fsanitize=address,undefined -fno-sanitize-recover=all"
FAST_FLAGS="$STD -O2"

mkdir -p build

echo "compiling $SOL ..."
$CXX $DEBUG_FLAGS -o build/_sol "$SOL" || exit 1
echo "compiling $BRUTE ..."
$CXX $FAST_FLAGS -o build/_brute "$BRUTE" || exit 1

for ((i = 1; i <= ITERS; i++)); do
    python3 "$GEN" "$i" > build/_in.txt

    if ! ./build/_sol < build/_in.txt > build/_out_sol.txt 2> build/_err.txt; then
        echo
        echo "=== solution crashed on test $i (seed $i) ==="
        cat build/_in.txt
        echo "--- stderr ---"
        cat build/_err.txt
        exit 1
    fi

    ./build/_brute < build/_in.txt > build/_out_brute.txt

    if ! diff -qZ build/_out_sol.txt build/_out_brute.txt > /dev/null; then
        echo
        echo "=== mismatch on test $i (seed $i) ==="
        echo "--- input ---"
        cat build/_in.txt
        echo "--- your output ---"
        cat build/_out_sol.txt
        echo "--- brute output ---"
        cat build/_out_brute.txt
        exit 1
    fi

    printf "\rpassed %d/%d" "$i" "$ITERS"
done

echo
echo "all $ITERS tests passed"
