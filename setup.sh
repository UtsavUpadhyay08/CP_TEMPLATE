#!/usr/bin/env bash
# One-time setup, run inside WSL (Ubuntu).
#
#   cd /mnt/c/Users/utsav/Documents/CP_TEMPLATE && bash setup.sh
#
# Installs the toolchain and copies this folder to ~/cp, which lives on the
# Linux filesystem and compiles noticeably faster than /mnt/c. It also puts
# cp.mk in place as the Makefile, vscode-config/ in place as .vscode/, and
# regenerates the algo-* snippets from lib/.

set -e

echo "==> installing toolchain"
sudo apt update
sudo apt install -y build-essential g++ python3 make

echo
echo "==> g++ version"
g++ --version | head -1

DEST="$HOME/cp"
SRC="$(cd "$(dirname "$0")" && pwd)"

if [ ! -f "$SRC/template.cpp" ] || [ ! -f "$SRC/cp.mk" ]; then
    echo "error: $SRC does not look like the CP template folder." >&2
    echo "cd into the folder holding template.cpp and run:  bash setup.sh" >&2
    exit 1
fi

if [ "$SRC" != "$DEST" ]; then
    echo
    echo "==> copying template to $DEST"
    mkdir -p "$DEST"
    (cd "$SRC" && tar --exclude=./build --exclude=./.git -cf - .) | (cd "$DEST" && tar -xf -)
fi

cd "$DEST"

echo "==> regenerating algo-* snippets from lib/"
python3 tools/gen_snippets.py

echo "==> installing Makefile and .vscode"
cp cp.mk Makefile
mkdir -p .vscode
cp vscode-config/settings.json vscode-config/tasks.json \
   vscode-config/c_cpp_properties.json \
   vscode-config/cp.code-snippets vscode-config/algo.code-snippets .vscode/

chmod +x stress.sh setup.sh tools/check.sh
cp -n template.cpp main.cpp 2>/dev/null || true
touch input.txt output.txt expected_output.txt

# Timestamps don't survive copying, so detect an outdated working file by the
# version marker in template.cpp rather than by mtime.
MARKER=$(grep -o 'TEMPLATE_V[0-9]*' template.cpp | head -1)
for working in main.cpp sol.cpp; do
    if [ -f "$working" ] && ! grep -q "$MARKER" "$working"; then
        echo
        echo "note: $working predates the current template ($MARKER)."
        echo "      Refresh it with:  cp template.cpp $working"
        echo "      (it will keep waiting on the keyboard instead of reading"
        echo "       input.txt until you do)"
    fi
done

# The smoke test always builds a throwaway copy of template.cpp, never sol.cpp:
# sol.cpp is yours to edit and may be anything. stdin is redirected and the run
# is bounded by a timeout so this step can never hang waiting for input.
echo
echo "==> smoke test (file mode)"
# Write BOTH files, so a fresh install starts in a state where the run button
# reports PASS. Writing only input.txt would leave a stale expected_output.txt
# describing a different problem, and the first run would FAIL for no reason.
printf '1\n3\n1 2 3\n' > input.txt
printf '3\n' > expected_output.txt
cp template.cpp _smoke.cpp
make f=_smoke > /dev/null
if timeout 10 ./build/_smoke < input.txt; then
    echo "input.txt  -> $(tr '\n' ' ' < input.txt)"
    echo "output.txt -> $(cat output.txt)"
else
    echo "!! smoke test failed or timed out (exit $?)" >&2
fi
rm -f _smoke.cpp

echo
echo "done. open the folder in VS Code with:  cd ~/cp && code ."
echo
echo "To get the run keybindings, open the Command Palette (Ctrl+Shift+P),"
echo "run 'Preferences: Open Keyboard Shortcuts (JSON)', and paste the"
echo "entries from vscode-config/keybindings.json."
