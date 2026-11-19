#pragma once
#include <bits/stdc++.h>

namespace nt {

// (base^e) % mod, safe for mod < 2^31
inline long long pw(long long b, long long e, long long mod) {
    long long r = 1 % mod;
    b %= mod;
    if (b < 0) b += mod;
    while (e > 0) {
        if (e & 1) r = (__int128)r * b % mod;
        b = (__int128)b * b % mod;
        e >>= 1;
    }
    return r;
}

// Linear sieve: smallest prime factor for every i <= n, plus the prime list.
struct Sieve {
    std::vector<int> spf, primes;

    explicit Sieve(int n) : spf(n + 1, 0) {
        for (int i = 2; i <= n; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > spf[i] || (long long)i * p > n) break;
                spf[(long long)i * p] = p;
            }
        }
    }

    bool is_prime(int x) const { return x >= 2 && spf[x] == x; }

    // prime -> exponent, using the precomputed spf table
    std::vector<std::pair<int, int>> factor(int x) const {
        std::vector<std::pair<int, int>> f;
        while (x > 1) {
            int p = spf[x], c = 0;
            while (x % p == 0) x /= p, ++c;
            f.emplace_back(p, c);
        }
        return f;
    }
};

// Trial division for a single number up to ~1e18 when no sieve is available.
inline std::vector<std::pair<long long, int>> factor(long long x) {
    std::vector<std::pair<long long, int>> f;
    for (long long p = 2; p * p <= x; ++p) {
        if (x % p) continue;
        int c = 0;
        while (x % p == 0) x /= p, ++c;
        f.emplace_back(p, c);
    }
    if (x > 1) f.emplace_back(x, 1);
    return f;
}

// extended gcd: returns g = gcd(a, b) with a*x + b*y = g
inline long long extgcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) { x = 1, y = 0; return a; }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// modular inverse for any mod coprime to a; returns -1 when it does not exist
inline long long inv_mod(long long a, long long mod) {
    long long x, y;
    long long g = extgcd(((a % mod) + mod) % mod, mod, x, y);
    if (g != 1) return -1;
    return ((x % mod) + mod) % mod;
}

}  // namespace nt
