#pragma once
#include <bits/stdc++.h>

// Fenwick (binary indexed) tree, 0-indexed interface. Point add, prefix sum.
template <class T = long long>
struct Fenwick {
    int n;
    std::vector<T> t;

    explicit Fenwick(int n_) : n(n_), t(n_ + 1, T{}) {}

    void add(int i, T v) {
        for (++i; i <= n; i += i & -i) t[i] += v;
    }

    // sum of [0, r)
    T sum(int r) const {
        T s{};
        for (; r > 0; r -= r & -r) s += t[r];
        return s;
    }

    // sum of [l, r)
    T sum(int l, int r) const { return sum(r) - sum(l); }

    // smallest i such that sum(i + 1) > target; -1 if none. Requires non-negative values.
    int lower_bound(T target) const {
        int pos = 0;
        T cur{};
        for (int pw = 1 << std::__lg(std::max(n, 1)); pw > 0; pw >>= 1) {
            if (pos + pw <= n && cur + t[pos + pw] <= target) {
                pos += pw;
                cur += t[pos];
            }
        }
        return pos == n ? -1 : pos;
    }
};
