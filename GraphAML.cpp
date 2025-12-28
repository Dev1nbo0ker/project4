#include "GraphAML.h"

#include <algorithm>
#include <iostream>
#include <queue>

GraphAML::GraphAML() : n_(0) {}

GraphAML::~GraphAML() {
    for (auto* e : edges_) {
        delete e;
    }
}

void GraphAML::Init(int n) {
    n_ = n;
    vertices_.assign(n_ + 1, {nullptr});
    for (auto* e : edges_) {
        delete e;
    }
    edges_.clear();
}

void GraphAML::AddEdge(int u, int v, int w) {
    if (u < 1 || v < 1 || u > n_ || v > n_) {
        return;
    }
    // 邻接多重表边结点，记录两个端点及两条链指针
    auto* edge = new AMLEdge{u, v, w, nullptr, nullptr};
    edge->ilink = vertices_[u].firstEdge;
    edge->jlink = vertices_[v].firstEdge;
    vertices_[u].firstEdge = edge;
    vertices_[v].firstEdge = edge;
    edges_.push_back(edge);
}

bool GraphAML::IsReady() const {
    return n_ > 0;
}

int GraphAML::VertexCount() const {
    return n_;
}

std::vector<int> GraphAML::CollectNeighbors(int v) const {
    std::vector<int> neighbors;
    AMLEdge* cur = vertices_[v].firstEdge;
    while (cur) {
        int other = (cur->ivex == v) ? cur->jvex : cur->ivex;
        neighbors.push_back(other);
        // 沿着属于 v 的那条链走
        cur = (cur->ivex == v) ? cur->ilink : cur->jlink;
    }
    std::sort(neighbors.begin(), neighbors.end());
    return neighbors;
}

void GraphAML::BFS(int start, std::vector<int>& order, std::vector<std::pair<int, int>>& treeEdges,
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
        // 通过 AML 收集邻居并排序，保证升序访问
        std::vector<int> neighbors = CollectNeighbors(v);
        for (int to : neighbors) {
            if (!visited[to]) {
                visited[to] = true;
                parent[to] = v;
                treeEdges.push_back({v, to});
                q.push(to);
            }
        }
    }
}
