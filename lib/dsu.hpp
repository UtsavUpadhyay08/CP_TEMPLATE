#pragma once
#include <bits/stdc++.h>

// Disjoint set union with union by size + path compression. O(alpha(n)) amortized.
struct DSU {
    std::vector<int> p, sz;
    int comps;

    explicit DSU(int n) : p(n), sz(n, 1), comps(n) {
        std::iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        while (p[x] != x) x = p[x] = p[p[x]];
        return x;
    }

    bool same(int a, int b) { return find(a) == find(b); }

    // returns false if a and b were already in the same component
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) std::swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        --comps;
        return true;
    }

    int size(int x) { return sz[find(x)]; }
};
