#pragma once
#include <bits/stdc++.h>

namespace str {

// z[i] = length of the longest common prefix of s and s.substr(i). z[0] = n.
inline std::vector<int> z_function(const std::string& s) {
    int n = (int)s.size();
    std::vector<int> z(n);
    if (n) z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = std::min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}

// pi[i] = length of the longest proper prefix of s[0..i] that is also a suffix.
inline std::vector<int> prefix_function(const std::string& s) {
    int n = (int)s.size();
    std::vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

// all start positions of pat inside txt
inline std::vector<int> find_all(const std::string& txt, const std::string& pat) {
    std::vector<int> res;
    if (pat.empty() || pat.size() > txt.size()) return res;
    std::string s = pat + '\1' + txt;
    std::vector<int> z = z_function(s);
    int m = (int)pat.size();
    for (int i = m + 1; i < (int)s.size(); ++i)
        if (z[i] >= m) res.push_back(i - m - 1);
    return res;
}

// Double polynomial hash. Collision-resistant enough for contest use.
struct Hash {
    static constexpr unsigned long long M1 = 1000000007ULL, M2 = 998244353ULL;
    static constexpr unsigned long long B1 = 131ULL, B2 = 137ULL;
    std::vector<unsigned long long> h1, h2, p1, p2;

    explicit Hash(const std::string& s) {
        int n = (int)s.size();
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        p1.assign(n + 1, 1); p2.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            h1[i + 1] = (h1[i] * B1 + (unsigned char)s[i]) % M1;
            h2[i + 1] = (h2[i] * B2 + (unsigned char)s[i]) % M2;
            p1[i + 1] = p1[i] * B1 % M1;
            p2[i + 1] = p2[i] * B2 % M2;
        }
    }

    // hash of [l, r)
    std::pair<unsigned long long, unsigned long long> get(int l, int r) const {
        unsigned long long a = (h1[r] + M1 * M1 - h1[l] * p1[r - l]) % M1;
        unsigned long long b = (h2[r] + M2 * M2 - h2[l] * p2[r - l]) % M2;
        return {a, b};
    }
};

}  // namespace str
