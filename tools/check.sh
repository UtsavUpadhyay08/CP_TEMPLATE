#!/usr/bin/env bash
# Run a USE_FILES build (input.txt -> output.txt) and compare the result with
# expected_output.txt. Called by `make r`, which is what the run button runs.
#
#   tools/check.sh build/main
#
# output.txt holds everything the program produced, debug() lines included.
# Those lines all start with "[dbg " and are stripped before the comparison,
# so leaving debug calls in can never turn a correct answer into a FAIL.

BIN="$1"

RED=$'\033[31m'
GREEN=$'\033[32m'
YELLOW=$'\033[33m'
DIM=$'\033[2m'
RESET=$'\033[0m'

DBG_RE='^\[dbg '

if [ ! -x "$BIN" ]; then
    echo "${RED}no executable at $BIN${RESET}" >&2
    exit 1
fi

[ -f input.txt ] || touch input.txt
[ -f expected_output.txt ] || touch expected_output.txt

start=$(date +%s%N)
timeout 10 "$BIN" < input.txt
rc=$?
ms=$(( ($(date +%s%N) - start) / 1000000 ))

if [ $rc -eq 124 ]; then
    echo "${RED}TIMEOUT  killed after 10s${RESET}"
    exit 124
fi
if [ $rc -ne 0 ]; then
    echo "${RED}RUNTIME ERROR  exit code $rc  (${ms}ms)${RESET}"
    echo "${DIM}the sanitizer message above, if any, says exactly what went wrong${RESET}"
    exit $rc
fi

# everything the program wrote, debug lines included, exactly as in the file
echo "${DIM}--- output.txt (${ms}ms) ---${RESET}"
cat output.txt

# the answer only: debug lines removed
ANSWER=$(mktemp)
trap 'rm -f "$ANSWER"' EXIT
grep -v "$DBG_RE" output.txt > "$ANSWER" || true

ndbg=$(grep -c "$DBG_RE" output.txt || true)
if [ "$ndbg" -gt 0 ]; then
    echo "${DIM}($ndbg debug line(s) ignored in the comparison)${RESET}"
fi

if [ ! -s expected_output.txt ]; then
    echo "${YELLOW}expected_output.txt is empty - paste the expected answer there to get a verdict${RESET}"
    exit 0
fi

# -B ignores blank-line differences, -Z ignores trailing whitespace:
# a missing final newline should not read as a wrong answer.
if diff -qBZ "$ANSWER" expected_output.txt > /dev/null 2>&1; then
    echo "${GREEN}PASS  matches expected_output.txt  (${ms}ms)${RESET}"
    exit 0
fi

echo "${RED}FAIL  differs from expected_output.txt  (${ms}ms)${RESET}"
echo "${DIM}--- differences, debug lines excluded (- expected, + yours) ---${RESET}"
diff -BZ -U0 expected_output.txt "$ANSWER" |
    tail -n +3 |
    head -40 |
    while IFS= read -r line; do
        case "$line" in
            ---*|+++*) ;;
            -*) echo "${GREEN}${line}${RESET}" ;;
            +*) echo "${RED}${line}${RESET}" ;;
            *)  echo "${DIM}${line}${RESET}" ;;
        esac
    done
exit 1
