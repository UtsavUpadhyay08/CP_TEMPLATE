// Obviously-correct slow solution. Same I/O format as sol.cpp.
// Write the dumbest thing that works: full enumeration, O(n!) is fine.
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int& x : a) cin >> x;

    cout << n << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    cin >> T;
    while (T--) solve();
    return 0;
}
