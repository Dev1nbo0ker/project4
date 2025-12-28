#ifndef GRAPH_AML_H
#define GRAPH_AML_H

#include <utility>
#include <vector>

struct AMLEdge {
    int ivex;
    int jvex;
    int weight;
    AMLEdge* ilink;
    AMLEdge* jlink;
};

struct AMLVNode {
    AMLEdge* firstEdge;
};

class GraphAML {
public:
    GraphAML();
    ~GraphAML();

    void Init(int n);
    void AddEdge(int u, int v, int w);
    bool IsReady() const;
    int VertexCount() const;

    void BFS(int start, std::vector<int>& order, std::vector<std::pair<int, int>>& treeEdges,
             std::vector<int>& parent) const;

private:
    int n_;
    std::vector<AMLVNode> vertices_;
    std::vector<AMLEdge*> edges_;

    std::vector<int> CollectNeighbors(int v) const;
};

#endif
