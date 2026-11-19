# CP setup

VS Code + WSL2 (Ubuntu) + g++ + CPH. Open it from inside WSL, not from Windows.

## First-time setup

1. **WSL2** — in an admin PowerShell: `wsl --install -d Ubuntu`, then reboot.

2. **Everything else** — in the Ubuntu terminal:
   ```
   cd /mnt/c/Users/utsav/Documents/CP_TEMPLATE
   bash setup.sh
   ```
   Installs g++/make/python3, copies the folder to `~/cp`, generates the
   `algo-*` snippets, installs `Makefile` and `.vscode/`, and runs a smoke test.

   Work out of `~/cp` — compiling from `/mnt/c` goes through a translation layer
   and is slower. Keep this Windows folder as the master copy, re-run `setup.sh`
   to push changes across.

   (`cp.mk` and `vscode-config/` carry those names because the Claude desktop app
   refuses to write a file called `Makefile` or anything under `.vscode/` from a
   remote session. `setup.sh` renames them.)

3. **VS Code extensions** — WSL, C/C++, Competitive Programming Helper. Open with
   `cd ~/cp && code .`

4. **Keybindings** — Ctrl+Shift+P → "Preferences: Open Keyboard Shortcuts (JSON)"
   → paste the entries from `vscode-config/keybindings.json`. VS Code only reads
   keybindings from your user profile, so this one step can't be automated.

5. **Competitive Companion** in Chrome (port 27121) if you want CPH to pull
   samples automatically.

6. **Check the CPH setting keys took** — Settings → search `cph`. Extensions
   rename keys between versions; if any read as unknown, set them from the UI.
   Also pick the right C++23 submission compiler from whatever Codeforces lists.

## The run button

| Key | What |
|-----|------|
| **Ctrl+Shift+B** or **Ctrl+Alt+R** | compile + run on `input.txt` → `output.txt`, compare with `expected_output.txt`, print the verdict |
| Ctrl+Alt+P | same but read stdin instead of `input.txt` |
| Ctrl+Alt+S | stress test the open file |
| Ctrl+Alt+B | run all CPH sample tests |

Ctrl+Shift+B works with no setup — it's the default build task. The others need
step 4 above.

Want a literal play button in the title bar: install the **Code Runner**
extension. `settings.json` already configures it with the same flags.

From the terminal the same things are:

```
make f=main        # build main.cpp
make r f=main      # build + run + verdict          <- the run button
make pipe f=main   # build + run on stdin
make fast f=main   # submission flags, no sanitizers
make listen        # fill input/expected from Competitive Companion
make sample n=2    # load another parsed sample
./stress.sh main.cpp
```

`f` may include a folder: `make r f=contests/div2/a`.

## Workflow

Three text files sit next to the code:

| File | Role |
|------|------|
| `input.txt` | paste the problem's sample input here |
| `expected_output.txt` | paste the problem's expected output here |
| `output.txt` | written by your program; compared against the expected one |

### Filling them automatically

Don't paste samples by hand. Run `make listen` (or the "listen for samples"
task) and leave it going, then click the green (+) in Competitive Companion on
any problem page. It writes:

```
B. Chess  2 sample(s), 2000 ms limit
  input.txt and expected_output.txt now hold sample 1
  others saved in tests/ -- load one with: make sample n=2
```

Competitive Companion must have a custom port matching the listener (10043)
enabled in its browser settings. It broadcasts to every enabled port, so this
and CPH (27121) can both receive the same click.

### Running

`main.cpp` is your working file, a copy of `template.cpp`. Press the run button
and you get one of:

```
PASS  matches expected_output.txt  (14ms)

FAIL  differs from expected_output.txt  (13ms)
--- first differences (- expected, + yours) ---
@@ -2 +2 @@
-99        <- green, what was expected
+2         <- red, what you printed

RUNTIME ERROR  exit code 134  (9ms)
TIMEOUT  killed after 10s
```

Leave `expected_output.txt` empty and it just prints your output with no verdict.

**Debug traces go into `output.txt` but are never compared.** In file mode
`debug()` output is interleaved into `output.txt` in the right order, so the file
holds everything the program produced:

```
[dbg L110] n, a = 3, {7, 8, 9}
3
[dbg L110] n, a = 2, {1, 2}
2
```

Every debug line starts with `[dbg `, and `tools/check.sh` strips those before
diffing — the verdict above is a PASS against an `expected_output.txt` of just
`3` and `2`. Leaving debug calls in can never cause a FAIL, and the diff shown on
a failure excludes them too. Blank lines and trailing whitespace are ignored as
well, so a missing final newline won't read as wrong.

In stdin mode (CPH, stress testing) debug stays on stderr and coloured, so it
never contaminates what CPH compares.

Any `.cpp` works — the run button builds whichever file is focused, in any
subfolder. `template.cpp` stays untouched as the thing you copy from.

## debug()

```cpp
debug(n, a);            // [L97] n, a = 3, {7, 8, 9}
debug(mp);              // {("a", 1), ("b", 2)}
debug(grid);            // {{1, 2}, {3, 4}}
debug(p, tup, s, flag); // (1, 2), (1, 'x', 2.5), "hello", true
```

Prints the variable names alongside the values, and handles vectors, maps, sets,
arrays, pairs, tuples and nesting of all of them.

It's gated on `_UTSAV_`:

- `make`, CPH and the stress harness all pass `-D_UTSAV_` → debug prints.
- Judges don't → `debug(...)` expands to `((void)0)` and costs nothing.

So never strip debug lines before submitting. That's the whole point.

Two macros, not one: `USE_FILES` controls the `input.txt`/`output.txt`
redirection separately, because CPH pipes samples through stdin and would break
if the program always read `input.txt`. `make` passes it; CPH and `stress.sh`
don't.

## algo-* snippets

Judges take a single file, so `#include "dsu.hpp"` is useless at submission time.
Instead the contents of `lib/` are compiled into snippets that paste the whole
implementation inline.

The trigger is `algo-`, **not** `/` or `\`. VS Code's suggestion popup only
opens on word characters, so a prefix starting with a slash won't fire reliably.
Type `algo` and the whole list appears; Ctrl+Space forces it open anywhere.

In any `.cpp`, type `algo` to see the list, or go straight to one:

```
algo-dsu        algo-fenwick     algo-segtree    algo-modint
algo-math       algo-strings     algo-graph      algo-debug
algo-template
```

then press Enter. The full code drops in at the cursor.

Added something to `lib/`? Regenerate:

```
python3 tools/gen_snippets.py
cp vscode-config/algo.code-snippets .vscode/
```

`lib/` is still on the include path (`-Ilib`), so `#include "dsu.hpp"` works for
local testing — just don't rely on it in the file you submit.

## Stress testing

The highest-value habit on the way to Expert. When a solution fails on test 40
and you can't see why:

1. Write `stress/brute.cpp` — the dumbest correct solution; exponential is fine.
2. Edit `stress/gen.py` so `n` stays tiny (<= 8). You want a counterexample you
   can read at a glance.
3. `./stress.sh a.cpp` (or Ctrl+Alt+S).

It stops at the first mismatch and prints the input with both outputs. It also
catches crashes and sanitizer hits, which often fire before any mismatch does.

## lib/

- `debug.hpp` — the `debug()` machinery (already inside `template.cpp`)
- `dsu.hpp` — union by size + path compression
- `fenwick.hpp` — point add, prefix sum, `lower_bound`
- `segtree.hpp` — iterative, any monoid, non-commutative safe
- `modint.hpp` — compile-time modulus, factorials/binomials in `Comb`
- `math.hpp` — modpow, linear sieve, factorization, extgcd, modular inverse
- `strings.hpp` — z-function, prefix function, substring search, double hashing
- `graph.hpp` — dijkstra, bfs, topological sort

Obvious gaps to fill as you hit them: lazy segment tree, max flow, SCC/bridges.
Add them when a problem forces you to — an untested library is a liability
mid-contest. Anything you add to `lib/` becomes an `algo-*` snippet on the next
`gen_snippets.py` run.

## Also worth having

- **cf-tool** — submit from the terminal.
- **Codeforces Problem Rating** / **CF Analytics** — ratings inline in lists.
- **clist.by** — contest calendar across judges.
