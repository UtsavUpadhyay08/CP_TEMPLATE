#pragma once
#include <bits/stdc++.h>

namespace graph {

constexpr long long LINF = 4e18;

using WGraph = std::vector<std::vector<std::pair<int, long long>>>;  // {to, weight}

// Dijkstra from src. Returns distances; unreachable nodes get LINF.
// Pass a non-null parent vector to recover paths.
inline std::vector<long long> dijkstra(const WGraph& g, int src,
                                       std::vector<int>* parent = nullptr) {
    int n = (int)g.size();
    std::vector<long long> dist(n, LINF);
    if (parent) parent->assign(n, -1);
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>,
                        std::greater<>>
        pq;
    dist[src] = 0;
    pq.emplace(0LL, src);
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                if (parent) (*parent)[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}

// Unweighted shortest paths. Unreachable nodes get -1.
inline std::vector<int> bfs(const std::vector<std::vector<int>>& g, int src) {
    std::vector<int> dist((int)g.size(), -1);
    std::queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
    return dist;
}

// Topological order of a DAG; empty vector if the graph has a cycle.
inline std::vector<int> topo_sort(const std::vector<std::vector<int>>& g) {
    int n = (int)g.size();
    std::vector<int> indeg(n, 0), order;
    for (int u = 0; u < n; ++u)
        for (int v : g[u]) ++indeg[v];
    std::queue<int> q;
    for (int u = 0; u < n; ++u)
        if (indeg[u] == 0) q.push(u);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);
    }
    if ((int)order.size() != n) order.clear();
    return order;
}

}  // namespace graph
