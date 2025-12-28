#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

struct EdgeInput {
    int u;
    int v;
    int w;
};

uint64_t MakeEdgeKey(int u, int v);

bool ReadGraphInteractive(int& n, int& m, std::vector<EdgeInput>& edges);
bool ReadGraphFromFile(const std::string& path, int& n, int& m, std::vector<EdgeInput>& edges);

void PrintEdgeList(const std::vector<std::pair<int, int>>& edges);
void PrintVisitOrder(const std::vector<int>& order);

void PrintTree(const std::vector<int>& parent, int root);
std::vector<int> RebuildPath(const std::vector<int>& parent, int s, int v);

#endif
