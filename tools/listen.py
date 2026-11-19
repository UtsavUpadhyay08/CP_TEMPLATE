#!/usr/bin/env python3
"""Listen for Competitive Companion and fill in the test files automatically.

    python3 tools/listen.py          # or:  make listen

Leave it running in a terminal. Click the green (+) in Competitive Companion on
any Codeforces / AtCoder / CodeChef problem page and this writes:

    input.txt            <- sample 1 input
    expected_output.txt  <- sample 1 expected output
    tests/N.in, N.out    <- every sample, so you can swap them in later

Swap in another sample with:  make sample n=2

Competitive Companion must be configured to send to this port. Open its
settings in the browser and make sure a custom port matching --port (10043 by
default) is enabled. CPH listens on 27121; Companion sends to every enabled
port, so both can run at the same time.
"""
import argparse
import json
import pathlib
import sys
from http.server import BaseHTTPRequestHandler, HTTPServer

ROOT = pathlib.Path(__file__).resolve().parent.parent

GREEN = "\033[32m"
DIM = "\033[2m"
YELLOW = "\033[33m"
RESET = "\033[0m"


def write_problem(payload: dict) -> None:
    name = payload.get("name", "unnamed problem")
    tests = payload.get("tests") or []

    if not tests:
        print(f"{YELLOW}{name}: no samples in the payload{RESET}", flush=True)
        return

    tests_dir = ROOT / "tests"
    tests_dir.mkdir(exist_ok=True)
    # clear samples from the previous problem so stale ones can't be used
    for old in tests_dir.glob("*.in"):
        old.unlink()
    for old in tests_dir.glob("*.out"):
        old.unlink()

    for i, test in enumerate(tests, start=1):
        (tests_dir / f"{i}.in").write_text(test.get("input", ""), encoding="utf-8")
        (tests_dir / f"{i}.out").write_text(test.get("output", ""), encoding="utf-8")

    (ROOT / "input.txt").write_text(tests[0].get("input", ""), encoding="utf-8")
    (ROOT / "expected_output.txt").write_text(tests[0].get("output", ""), encoding="utf-8")

    limit = payload.get("timeLimit")
    limit_s = f", {limit} ms limit" if limit else ""
    print(f"{GREEN}{name}{RESET}  {len(tests)} sample(s){limit_s}", flush=True)
    print(f"{DIM}  input.txt and expected_output.txt now hold sample 1{RESET}", flush=True)
    if len(tests) > 1:
        print(f"{DIM}  others saved in tests/ -- load one with: make sample n=2{RESET}", flush=True)


class Handler(BaseHTTPRequestHandler):
    def do_POST(self):  # noqa: N802  (name fixed by BaseHTTPRequestHandler)
        length = int(self.headers.get("Content-Length", 0))
        raw = self.rfile.read(length)
        self.send_response(200)
        self.end_headers()
        try:
            write_problem(json.loads(raw.decode("utf-8")))
        except (json.JSONDecodeError, UnicodeDecodeError) as exc:
            print(f"{YELLOW}ignored a payload that was not valid JSON: {exc}{RESET}", flush=True)

    def log_message(self, *_args):
        pass  # the writes above are the only output worth seeing


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", type=int, default=10043)
    args = ap.parse_args()

    try:
        server = HTTPServer(("127.0.0.1", args.port), Handler)
    except OSError as exc:
        print(f"cannot listen on port {args.port}: {exc}", file=sys.stderr, flush=True)
        print("another listener is probably already running.", file=sys.stderr, flush=True)
        return 1

    print(f"listening on http://127.0.0.1:{args.port} for Competitive Companion", flush=True)
    print(f"{DIM}click the green (+) on a problem page. Ctrl+C to stop.{RESET}", flush=True)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
