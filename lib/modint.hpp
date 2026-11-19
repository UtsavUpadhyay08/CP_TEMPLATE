#pragma once
#include <bits/stdc++.h>

// Compile-time modulus modular integer. MOD must be prime for inv()/operator/.
template <unsigned MOD>
struct ModInt {
    unsigned v;

    constexpr ModInt(long long x = 0) : v(0) {
        long long r = x % (long long)MOD;
        if (r < 0) r += MOD;
        v = (unsigned)r;
    }

    static constexpr unsigned mod() { return MOD; }

    ModInt& operator+=(const ModInt& o) { if ((v += o.v) >= MOD) v -= MOD; return *this; }
    ModInt& operator-=(const ModInt& o) { if ((v += MOD - o.v) >= MOD) v -= MOD; return *this; }
    ModInt& operator*=(const ModInt& o) { v = (unsigned)((unsigned long long)v * o.v % MOD); return *this; }
    ModInt& operator/=(const ModInt& o) { return *this *= o.inv(); }

    friend ModInt operator+(ModInt a, const ModInt& b) { return a += b; }
    friend ModInt operator-(ModInt a, const ModInt& b) { return a -= b; }
    friend ModInt operator*(ModInt a, const ModInt& b) { return a *= b; }
    friend ModInt operator/(ModInt a, const ModInt& b) { return a /= b; }
    ModInt operator-() const { return ModInt() - *this; }

    friend bool operator==(const ModInt& a, const ModInt& b) { return a.v == b.v; }
    friend bool operator!=(const ModInt& a, const ModInt& b) { return a.v != b.v; }

    ModInt pow(long long e) const {
        ModInt b = *this, r = 1;
        if (e < 0) { b = b.inv(); e = -e; }
        while (e) { if (e & 1) r *= b; b *= b; e >>= 1; }
        return r;
    }

    ModInt inv() const { return pow((long long)MOD - 2); }

    friend std::ostream& operator<<(std::ostream& os, const ModInt& a) { return os << a.v; }
    friend std::istream& operator>>(std::istream& is, ModInt& a) { long long x; is >> x; a = ModInt(x); return is; }
};

using mint = ModInt<998244353>;
// using mint = ModInt<1000000007>;

// Factorials / binomials. Call Comb::init(n) once before use.
namespace Comb {
inline std::vector<mint> fact, inv_fact;

inline void init(int n) {
    fact.assign(n + 1, 1);
    inv_fact.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    inv_fact[n] = fact[n].inv();
    for (int i = n; i > 0; --i) inv_fact[i - 1] = inv_fact[i] * i;
}

inline mint C(int n, int k) {
    if (k < 0 || k > n || n < 0) return 0;
    return fact[n] * inv_fact[k] * inv_fact[n - k];
}

inline mint P(int n, int k) {
    if (k < 0 || k > n || n < 0) return 0;
    return fact[n] * inv_fact[n - k];
}
}  // namespace Comb
