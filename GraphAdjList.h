#ifndef GRAPH_ADJLIST_H
#define GRAPH_ADJLIST_H

#include <string>
#include <utility>
#include <vector>

struct AdjEdge {
    int to;
    int weight;
};

class GraphAdjList {
public:
    GraphAdjList();

    void Init(int n);
    void AddEdge(int u, int v, int w);
    void SortAdjacency();

    bool IsReady() const;
    int VertexCount() const;

    void Show() const;
    void ExportGraphDot(const std::string& path) const;

    void BFS(int start, std::vector<int>& order, std::vector<std::pair<int, int>>& treeEdges,
             std::vector<int>& parent) const;
    void DFSIterative(int start, std::vector<int>& order, std::vector<std::pair<int, int>>& treeEdges,
                      std::vector<int>& parent) const;

    void ExportTreeDot(const std::string& path, const std::vector<std::pair<int, int>>& treeEdges) const;

    void Dijkstra(int start, std::vector<int>& parent, std::vector<long long>& dist) const;
    void ExportShortestPathDot(const std::string& path, int s, int t,
                               const std::vector<int>& parent) const;

    const std::vector<std::vector<AdjEdge>>& Adj() const;

private:
    int n_;
    std::vector<std::vector<AdjEdge>> adj_;
};

#endif
