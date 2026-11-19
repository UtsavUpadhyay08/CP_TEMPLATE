#pragma once
#include <bits/stdc++.h>

// Iterative segment tree over any monoid (op, identity). Non-commutative safe.
// Usage:
//   SegTree st(n, 0LL, [](long long a, long long b) { return a + b; });
//   st.build(values);  st.set(i, v);  st.query(l, r);   // half-open [l, r)
template <class T, class Op>
struct SegTree {
    int n;
    T id;
    Op op;
    std::vector<T> t;

    SegTree(int n_, T id_, Op op_) : n(n_), id(id_), op(op_), t(2 * n_, id_) {}

    void build(const std::vector<T>& a) {
        for (int i = 0; i < n; ++i) t[n + i] = a[i];
        for (int i = n - 1; i > 0; --i) t[i] = op(t[2 * i], t[2 * i + 1]);
    }

    void set(int i, T v) {
        for (t[i += n] = v; i > 1; i >>= 1) t[i >> 1] = op(t[i & ~1], t[i | 1]);
    }

    T get(int i) const { return t[i + n]; }

    // fold over [l, r)
    T query(int l, int r) const {
        T resl = id, resr = id;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = op(resl, t[l++]);
            if (r & 1) resr = op(t[--r], resr);
        }
        return op(resl, resr);
    }
};

template <class T, class Op>
SegTree(int, T, Op) -> SegTree<T, Op>;
