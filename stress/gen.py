#!/usr/bin/env python3
"""Random test generator for stress.sh.

Called as:  python3 gen.py <seed>
Keep the generated tests SMALL -- the point is to find a counterexample
you can read, not to benchmark. Edit the body for each problem.
"""
import random
import sys

rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 0)

# ---- edit below for the problem at hand ----
t = 1
print(t)
for _ in range(t):
    n = rnd.randint(1, 8)
    print(n)
    print(*[rnd.randint(1, 10) for _ in range(n)])
