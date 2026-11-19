#pragma once
// Container-aware debug printing, identical to the block inside template.cpp.
// Only active with -D_UTSAV_, so debug() lines are safe to leave in what you
// submit. Every line starts with "[dbg " so tools/check.sh can strip them
// before comparing output.txt with expected_output.txt.
#include <bits/stdc++.h>

#ifdef _UTSAV_

namespace dbgx {

template <class T, class = void>
struct is_iterable : std::false_type {};
template <class T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<const T&>())),
                                  decltype(std::end(std::declval<const T&>()))>>
    : std::true_type {};

template <class T>
void pr(const T& x) {
    if constexpr (std::is_convertible_v<T, std::string_view>) {
        std::cerr << '"' << std::string_view(x) << '"';
    } else if constexpr (std::is_same_v<T, char>) {
        std::cerr << '\'' << x << '\'';
    } else if constexpr (std::is_same_v<T, bool>) {
        std::cerr << (x ? "true" : "false");
    } else if constexpr (requires { x.first; x.second; }) {
        std::cerr << '(';
        pr(x.first);
        std::cerr << ", ";
        pr(x.second);
        std::cerr << ')';
    } else if constexpr (is_iterable<T>::value) {
        std::cerr << '{';
        bool first = true;
        for (const auto& e : x) {
            if (!first) std::cerr << ", ";
            first = false;
            pr(e);
        }
        std::cerr << '}';
    } else if constexpr (requires { std::tuple_size<T>::value; }) {
        std::cerr << '(';
        bool first = true;
        std::apply([&](const auto&... a) { ((std::cerr << (first ? "" : ", "), first = false, pr(a)), ...); }, x);
        std::cerr << ')';
    } else {
        std::cerr << x;
    }
}

inline void pr_all() {}
template <class T, class... R>
void pr_all(const T& x, const R&... rest) {
    pr(x);
    if constexpr (sizeof...(rest) > 0) {
        std::cerr << ", ";
        pr_all(rest...);
    }
}

}  // namespace dbgx

#ifdef USE_FILES
#define DBG_ON ""
#define DBG_OFF ""
#else
#define DBG_ON "\033[33m"
#define DBG_OFF "\033[0m"
#endif

#define debug(...)                                                                     \
    (std::cerr << DBG_ON "[dbg L" << __LINE__ << "] " << #__VA_ARGS__ << " = " DBG_OFF, \
     dbgx::pr_all(__VA_ARGS__), std::cerr << std::endl)

#else
#define debug(...) ((void)0)
#endif
