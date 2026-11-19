#!/usr/bin/env python3
"""Turn lib/*.hpp into VS Code snippets so whole algorithms can be pasted
inline -- online judges take a single file, so #include "dsu.hpp" is no use
at submission time.

    python3 tools/gen_snippets.py

Writes vscode-config/algo.code-snippets. Re-run it whenever you add or edit
something in lib/, then re-run setup.sh (or copy the file into .vscode/).

In the editor: type `algo` to see every structure, or `algo-dsu` for one,
then Enter.
"""
import json
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
LIB = ROOT / "lib"
OUT = ROOT / "vscode-config" / "algo.code-snippets"

# lines dropped from every header: they are already in the template
DROP_PREFIXES = ("#pragma once", "#include <bits/stdc++.h>")


def escape(line: str) -> str:
    """Escape the characters VS Code treats as snippet syntax."""
    return line.replace("\\", "\\\\").replace("$", "\\$")


def body_from(path: pathlib.Path) -> list[str]:
    lines = path.read_text(encoding="utf-8").splitlines()
    kept = [ln for ln in lines if not ln.strip().startswith(DROP_PREFIXES)]
    # drop leading and trailing blank lines
    while kept and not kept[0].strip():
        kept.pop(0)
    while kept and not kept[-1].strip():
        kept.pop()
    return [escape(ln) for ln in kept]


def describe(path: pathlib.Path) -> str:
    """First real comment line in the file, used as the snippet description."""
    for ln in path.read_text(encoding="utf-8").splitlines():
        s = ln.strip()
        if s.startswith("//") and len(s) > 3:
            return s[2:].strip()
    return path.stem


def main() -> int:
    if not LIB.is_dir():
        print(f"error: {LIB} not found", file=sys.stderr)
        return 1

    snippets = {}

    for path in sorted(LIB.glob("*.hpp")):
        name = path.stem
        snippets[f"algo: {name}"] = {
            "scope": "cpp",
            "prefix": f"algo-{name}",
            "body": body_from(path),
            "description": describe(path),
        }

    tpl = ROOT / "template.cpp"
    if tpl.is_file():
        snippets["algo: template"] = {
            "scope": "cpp",
            "prefix": "algo-template",
            "body": [escape(ln) for ln in tpl.read_text(encoding="utf-8").splitlines()],
            "description": "Full contest template with debug() and file I/O",
        }

    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(json.dumps(snippets, indent=2) + "\n", encoding="utf-8")

    print(f"wrote {OUT.relative_to(ROOT)} with {len(snippets)} snippets:")
    for key in snippets:
        print(f"  {key:22} -> {snippets[key]['prefix']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
