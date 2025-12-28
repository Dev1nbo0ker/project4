#include "GraphAdjList.h"

#include "MyStack.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>

GraphAdjList::GraphAdjList() : n_(0) {}

void GraphAdjList::Init(int n) {
    n_ = n;
    adj_.assign(n_ + 1, {});
}

void GraphAdjList::AddEdge(int u, int v, int w) {
    if (u < 1 || v < 1 || u > n_ || v > n_) {
        return;
    }
    adj_[u].push_back({v, w});
    adj_[v].push_back({u, w});
}

void GraphAdjList::SortAdjacency() {
    for (auto& list : adj_) {
        std::sort(list.begin(), list.end(), [](const AdjEdge& a, const AdjEdge& b) {
            return a.to < b.to;
        });
    }
}

bool GraphAdjList::IsReady() const {
    return n_ > 0;
}

int GraphAdjList::VertexCount() const {
    return n_;
}

const std::vector<std::vector<AdjEdge>>& GraphAdjList::Adj() const {
    return adj_;
}

void GraphAdjList::Show() const {
    for (int v = 1; v <= n_; ++v) {
        std::cout << v << ":";
        for (const auto& e : adj_[v]) {
            std::cout << " (" << e.to << "," << e.weight << ")";
        }
        std::cout << "\n";
    }
}

void GraphAdjList::ExportGraphDot(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cout << "无法写入 dot 文件.\n";
        return;
    }
    out << "graph G {\n";
    for (int v = 1; v <= n_; ++v) {
        out << "  " << v << ";\n";
    }
    std::unordered_set<long long> seen;
    for (int u = 1; u <= n_; ++u) {
        for (const auto& e : adj_[u]) {
            int v = e.to;
            int a = u < v ? u : v;
            int b = u < v ? v : u;
            long long key = (static_cast<long long>(a) << 32) | static_cast<unsigned int>(b);
            if (seen.count(key) > 0) {
                continue;
            }
            seen.insert(key);
            out << "  " << a << " -- " << b << " [label=\"" << e.weight << "\"];\n";
        }
    }
    out << "}\n";
}

void GraphAdjList::BFS(int start, std::vector<int>& order, std::vector<std::pair<int, int>>& treeEdges,
                       std::vector<int>& parent) const {
    order.clear();
    treeEdges.clear();
    parent.assign(n_ + 1, 0);

    std::vector<bool> visited(n_ + 1, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);
        for (const auto& e : adj_[v]) {
            int to = e.to;
            if (!visited[to]) {
                visited[to] = true;
                parent[to] = v;
                treeEdges.push_back({v, to});
                q.push(to);
            }
        }
    }
}

void GraphAdjList::DFSIterative(int start, std::vector<int>& order,
                                std::vector<std::pair<int, int>>& treeEdges,
                                std::vector<int>& parent) const {
    order.clear();
    treeEdges.clear();
    parent.assign(n_ + 1, 0);
    std::vector<bool> visited(n_ + 1, false);

    struct Frame {
        int v;
        size_t nextIdx;
    };

    MyStack<Frame> stack;
    stack.push({start, 0});
    visited[start] = true;
    order.push_back(start);

    while (!stack.empty()) {
        Frame& frame = stack.top();
        int v = frame.v;
        if (frame.nextIdx >= adj_[v].size()) {
            stack.pop();
            continue;
        }

        const AdjEdge& edge = adj_[v][frame.nextIdx];
        ++frame.nextIdx;
        int to = edge.to;
        if (!visited[to]) {
            visited[to] = true;
            parent[to] = v;
            treeEdges.push_back({v, to});
            order.push_back(to);
            // 使用栈帧保存“下一条边索引”，严格模拟递归 DFS 的回溯顺序
            stack.push({to, 0});
        }
    }
}

void GraphAdjList::ExportTreeDot(const std::string& path, const std::vector<std::pair<int, int>>& treeEdges) const {
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cout << "无法写入 dot 文件.\n";
        return;
    }
    out << "digraph T {\n";
    for (int v = 1; v <= n_; ++v) {
        out << "  " << v << ";\n";
    }
    for (const auto& e : treeEdges) {
        out << "  " << e.first << " -> " << e.second << ";\n";
    }
    out << "}\n";
}

void GraphAdjList::Dijkstra(int start, std::vector<int>& parent, std::vector<long long>& dist) const {
    const long long INF = static_cast<long long>(4e18);
    dist.assign(n_ + 1, INF);
    parent.assign(n_ + 1, 0);

    using Node = std::pair<long long, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();
        if (d != dist[v]) {
            continue;
        }
        for (const auto& e : adj_[v]) {
            int to = e.to;
            long long nd = d + e.weight;
            // Dijkstra 松弛：若找到更短距离则更新 parent
            if (nd < dist[to]) {
                dist[to] = nd;
                parent[to] = v;
                pq.push({nd, to});
            }
        }
    }
}

void GraphAdjList::ExportShortestPathDot(const std::string& path, int s, int t,
                                         const std::vector<int>& parent) const {
    std::vector<int> onPath(n_ + 1, 0);
    int cur = t;
    while (cur != 0) {
        onPath[cur] = 1;
        if (cur == s) {
            break;
        }
        cur = parent[cur];
    }

    std::set<std::pair<int, int>> pathEdges;
    cur = t;
    while (cur != 0 && cur != s) {
        int p = parent[cur];
        if (p == 0) {
            break;
        }
        int a = p < cur ? p : cur;
        int b = p < cur ? cur : p;
        pathEdges.insert({a, b});
        cur = p;
    }

    std::ofstream out(path);
    if (!out.is_open()) {
        std::cout << "无法写入 dot 文件.\n";
        return;
    }
    out << "graph G {\n";
    for (int v = 1; v <= n_; ++v) {
        out << "  " << v << ";\n";
    }

    std::unordered_set<long long> seen;
    for (int u = 1; u <= n_; ++u) {
        for (const auto& e : adj_[u]) {
            int v = e.to;
            int a = u < v ? u : v;
            int b = u < v ? v : u;
            long long key = (static_cast<long long>(a) << 32) | static_cast<unsigned int>(b);
            if (seen.count(key) > 0) {
                continue;
            }
            seen.insert(key);
            bool highlight = pathEdges.count({a, b}) > 0;
            if (highlight) {
                // 最短路径边高亮显示
                out << "  " << a << " -- " << b
                    << " [label=\"" << e.weight << "\", color=red, penwidth=2];\n";
            } else {
                out << "  " << a << " -- " << b
                    << " [label=\"" << e.weight << "\", color=gray];\n";
            }
        }
    }
    out << "}\n";
}
