// TEMPLATE_V2 -- setup.sh looks for this marker to spot working files copied
// from an older template. Bump it if you change the template substantially.
#include <bits/stdc++.h>
using namespace std;

// ===================== debug =====================
// Everything here disappears unless -D_UTSAV_ is passed, so debug() lines are
// safe to leave in the code you submit.
#ifdef _UTSAV_

namespace dbgx {

template <class T, class = void>
struct is_iterable : false_type {};
template <class T>
struct is_iterable<T, void_t<decltype(begin(declval<const T&>())), decltype(end(declval<const T&>()))>>
    : true_type {};

template <class T>
void pr(const T& x) {
    if constexpr (is_convertible_v<T, string_view>) {
        cerr << '"' << string_view(x) << '"';
    } else if constexpr (is_same_v<T, char>) {
        cerr << '\'' << x << '\'';
    } else if constexpr (is_same_v<T, bool>) {
        cerr << (x ? "true" : "false");
    } else if constexpr (requires { x.first; x.second; }) {
        cerr << '(';
        pr(x.first);
        cerr << ", ";
        pr(x.second);
        cerr << ')';
    } else if constexpr (is_iterable<T>::value) {
        cerr << '{';
        bool first = true;
        for (const auto& e : x) {
            if (!first) cerr << ", ";
            first = false;
            pr(e);
        }
        cerr << '}';
    } else if constexpr (requires { tuple_size<T>::value; }) {
        cerr << '(';
        bool first = true;
        apply([&](const auto&... a) { ((cerr << (first ? "" : ", "), first = false, pr(a)), ...); }, x);
        cerr << ')';
    } else {
        cerr << x;
    }
}

inline void pr_all() {}
template <class T, class... R>
void pr_all(const T& x, const R&... rest) {
    pr(x);
    if constexpr (sizeof...(rest) > 0) {
        cerr << ", ";
        pr_all(rest...);
    }
}

}  // namespace dbgx

// In file mode the debug lines are written into output.txt alongside the real
// output, so colours would only show up as escape junk in the file. Every line
// starts with "[dbg " so the checker can strip them before comparing.
#ifdef USE_FILES
#define DBG_ON ""
#define DBG_OFF ""
#else
#define DBG_ON "\033[33m"
#define DBG_OFF "\033[0m"
#endif

#define debug(...)                                                               \
    (cerr << DBG_ON "[dbg L" << __LINE__ << "] " << #__VA_ARGS__ << " = " DBG_OFF, \
     dbgx::pr_all(__VA_ARGS__), cerr << endl)

#else
#define debug(...) ((void)0)
#endif
// =================== end debug ===================

using ll  = long long;
using ull = unsigned long long;
using ld  = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;
template <class T> using minpq = priority_queue<T, vector<T>, greater<T>>;

#define all(x)  begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
#define sz(x)   (int)(x).size()
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define per(i, a, b) for (int i = (b) - 1; i >= (a); --i)

template <class T> bool ckmin(T& a, const T& b) { return b < a ? a = b, true : false; }
template <class T> bool ckmax(T& a, const T& b) { return a < b ? a = b, true : false; }

constexpr int INF  = 1e9 + 7;
constexpr ll  LINF = 4e18;

void solve() {
    int n;
    cin >> n;
    vi a(n);
    for (int& x : a) cin >> x;
    debug(n, a);

    cout << n << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

// Reads input.txt and writes output.txt. Only active with -DUSE_FILES, which
// `make` passes and CPH does not -- CPH feeds samples through stdin.
#ifdef USE_FILES
    if (!freopen("input.txt", "r", stdin)) cerr << "!! input.txt not found\n";
    if (!freopen("output.txt", "w", stdout)) cerr << "!! cannot write output.txt\n";
#ifdef _UTSAV_
    // send debug() into output.txt too, interleaved in the right order.
    // tools/check.sh strips the [dbg ...] lines before comparing.
    cerr.rdbuf(cout.rdbuf());
#endif
#endif

    int T = 1;
    cin >> T;              // <-- delete this line for single-test problems
    while (T--) solve();
    return 0;
}
