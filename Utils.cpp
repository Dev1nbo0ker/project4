#include "Utils.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_set>

uint64_t MakeEdgeKey(int u, int v) {
    int a = u < v ? u : v;
    int b = u < v ? v : u;
    return (static_cast<uint64_t>(a) << 32) | static_cast<uint32_t>(b);
}

static bool ReadGraphFromStream(std::istream& in, int& n, int& m, std::vector<EdgeInput>& edges) {
    if (!(in >> n >> m)) {
        return false;
    }
    if (n <= 0 || m < 0) {
        std::cout << "输入的 n 或 m 不合法.\n";
        return false;
    }

    edges.clear();
    edges.reserve(static_cast<size_t>(m));
    std::unordered_set<uint64_t> seen;

    for (int i = 0; i < m; ++i) {
        EdgeInput e{};
        if (!(in >> e.u >> e.v >> e.w)) {
            std::cout << "边输入不足.\n";
            return false;
        }
        if (e.u < 1 || e.u > n || e.v < 1 || e.v > n) {
            std::cout << "存在越界顶点.\n";
            return false;
        }
        if (e.u == e.v) {
            std::cout << "不允许自环.\n";
            return false;
        }
        if (e.w <= 0) {
            std::cout << "权重必须为正.\n";
            return false;
        }
        uint64_t key = MakeEdgeKey(e.u, e.v);
        if (seen.count(key) > 0) {
            std::cout << "检测到重边.\n";
            return false;
        }
        seen.insert(key);
        edges.push_back(e);
    }
    return true;
}

bool ReadGraphInteractive(int& n, int& m, std::vector<EdgeInput>& edges) {
    std::cout << "请输入 n m:\n";
    return ReadGraphFromStream(std::cin, n, m, edges);
}

bool ReadGraphFromFile(const std::string& path, int& n, int& m, std::vector<EdgeInput>& edges) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cout << "无法打开文件.\n";
        return false;
    }
    return ReadGraphFromStream(fin, n, m, edges);
}

void PrintEdgeList(const std::vector<std::pair<int, int>>& edges) {
    for (const auto& e : edges) {
        std::cout << e.first << " -> " << e.second << "\n";
    }
}

void PrintVisitOrder(const std::vector<int>& order) {
    for (size_t i = 0; i < order.size(); ++i) {
        if (i > 0) {
            std::cout << " ";
        }
        std::cout << order[i];
    }
    std::cout << "\n";
}

static void PrintTreeRecursive(int v, const std::vector<std::vector<int>>& children, int depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << v << "\n";
    for (int child : children[v]) {
        PrintTreeRecursive(child, children, depth + 1);
    }
}

void PrintTree(const std::vector<int>& parent, int root) {
    if (root < 1 || root >= static_cast<int>(parent.size())) {
        std::cout << "树根不合法.\n";
        return;
    }
    std::vector<std::vector<int>> children(parent.size());
    for (size_t v = 1; v < parent.size(); ++v) {
        int p = parent[v];
        if (p > 0) {
            children[p].push_back(static_cast<int>(v));
        }
    }
    for (auto& list : children) {
        std::sort(list.begin(), list.end());
    }
    PrintTreeRecursive(root, children, 0);
}

std::vector<int> RebuildPath(const std::vector<int>& parent, int s, int v) {
    std::vector<int> path;
    if (v < 1 || v >= static_cast<int>(parent.size())) {
        return path;
    }
    int cur = v;
    while (cur != 0) {
        path.push_back(cur);
        if (cur == s) {
            break;
        }
        cur = parent[cur];
    }
    if (path.back() != s) {
        path.clear();
        return path;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
