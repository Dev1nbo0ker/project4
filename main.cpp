#include "GraphAdjList.h"
#include "GraphAML.h"
#include "Utils.h"

#include <iostream>
#include <limits>
#include <string>

static void ShowMenu() {
    std::cout << "\n=== 图结构应用 ===\n";
    std::cout << "1. 交互式建图\n";
    std::cout << "2. 文件建图\n";
    std::cout << "3. 显示图 + 导出 graph.dot\n";
    std::cout << "4. BFS（AML）\n";
    std::cout << "5. 非递归 DFS（自定义栈）\n";
    std::cout << "6. 生成树树形输出\n";
    std::cout << "7. 最短路径（Dijkstra）\n";
    std::cout << "0. 退出\n";
    std::cout << "请选择:";
}

static bool BuildGraph(GraphAdjList& adj, GraphAML& aml, int& n, std::vector<EdgeInput>& edges) {
    adj.Init(n);
    aml.Init(n);
    for (const auto& e : edges) {
        adj.AddEdge(e.u, e.v, e.w);
        aml.AddEdge(e.u, e.v, e.w);
    }
    adj.SortAdjacency();
    return true;
}

int main() {
    GraphAdjList adj;
    GraphAML aml;
    int n = 0;
    int m = 0;
    std::vector<EdgeInput> edges;

    std::vector<int> bfsParent;
    std::vector<std::pair<int, int>> bfsTreeEdges;
    std::vector<int> bfsOrder;

    std::vector<int> dfsParent;
    std::vector<std::pair<int, int>> dfsTreeEdges;
    std::vector<int> dfsOrder;

    const int defaultStart = 1;

    while (true) {
        ShowMenu();
        int choice = -1;
        if (!(std::cin >> choice)) {
            return 0;
        }
        if (choice == 0) {
            break;
        }

        if (choice == 1) {
            if (!ReadGraphInteractive(n, m, edges)) {
                continue;
            }
            BuildGraph(adj, aml, n, edges);
            std::cout << "建图完成.\n";
        } else if (choice == 2) {
            std::cout << "请输入文件路径:";
            std::string path;
            std::cin >> path;
            if (!ReadGraphFromFile(path, n, m, edges)) {
                continue;
            }
            BuildGraph(adj, aml, n, edges);
            std::cout << "建图完成.\n";
        } else if (choice == 3) {
            if (!adj.IsReady()) {
                std::cout << "请先建图.\n";
                continue;
            }
            adj.Show();
            adj.ExportGraphDot("graph.dot");
            std::cout << "已导出 graph.dot\n";
        } else if (choice == 4) {
            if (!aml.IsReady()) {
                std::cout << "请先建图.\n";
                continue;
            }
            aml.BFS(defaultStart, bfsOrder, bfsTreeEdges, bfsParent);
            std::cout << "BFS 访问序列:";
            PrintVisitOrder(bfsOrder);
            std::cout << "BFS 生成树边集:\n";
            PrintEdgeList(bfsTreeEdges);
            adj.ExportTreeDot("bfs_tree.dot", bfsTreeEdges);
            std::cout << "已导出 bfs_tree.dot\n";
        } else if (choice == 5) {
            if (!adj.IsReady()) {
                std::cout << "请先建图.\n";
                continue;
            }
            adj.DFSIterative(defaultStart, dfsOrder, dfsTreeEdges, dfsParent);
            std::cout << "DFS 访问序列:";
            PrintVisitOrder(dfsOrder);
            std::cout << "DFS 生成树边集.\n";
            PrintEdgeList(dfsTreeEdges);
            adj.ExportTreeDot("dfs_tree.dot", dfsTreeEdges);
            std::cout << "已导出 dfs_tree.dot\n";
        } else if (choice == 6) {
            if (!adj.IsReady()) {
                std::cout << "请先建图.\n";
                continue;
            }
            if (bfsParent.empty()) {
                std::cout << "请先执行 BFS.\n";
            } else {
                std::cout << "BFS 生成树:\n";
                PrintTree(bfsParent, defaultStart);
            }
            if (dfsParent.empty()) {
                std::cout << "请先执行 DFS.\n";
            } else {
                std::cout << "DFS 生成树:\n";
                PrintTree(dfsParent, defaultStart);
            }
        } else if (choice == 7) {
            if (!adj.IsReady()) {
                std::cout << "请先建图.\n";
                continue;
            }
            int s = defaultStart;
            std::cout << "请输入起点 s（默认 1）:";
            if (!(std::cin >> s)) {
                return 0;
            }
            if (s < 1 || s > adj.VertexCount()) {
                std::cout << "起点不合法.\n";
                continue;
            }
            std::vector<int> parent;
            std::vector<long long> dist;
            adj.Dijkstra(s, parent, dist);

            std::cout << "最短距离与路径:\n";
            for (int v = 1; v <= adj.VertexCount(); ++v) {
                std::cout << "s -> " << v << " 距离 = " << dist[v] << "，路径:";
                std::vector<int> path = RebuildPath(parent, s, v);
                for (size_t i = 0; i < path.size(); ++i) {
                    if (i > 0) {
                        std::cout << "->";
                    }
                    std::cout << path[i];
                }
                std::cout << "\n";
            }

            std::cout << "请输入终点 t:";
            int t = 0;
            std::cin >> t;
            if (t < 1 || t > adj.VertexCount()) {
                std::cout << "终点不合法.\n";
                continue;
            }
            std::vector<int> path = RebuildPath(parent, s, t);
            std::cout << "s -> t 路径:";
            for (size_t i = 0; i < path.size(); ++i) {
                if (i > 0) {
                    std::cout << "->";
                }
                std::cout << path[i];
            }
            std::cout << "，总长度 = " << dist[t] << "\n";
            adj.ExportShortestPathDot("shortest_path.dot", s, t, parent);
            std::cout << "已导出 shortest_path.dot\n";
        } else {
            std::cout << "无效选项.\n";
        }

        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

    return 0;
}
