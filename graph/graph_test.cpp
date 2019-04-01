#include <iostream>
#include "graph.h"

int main() {
    Graph g;
    g.add_edge(5, 2);
    g.add_edge(5, 0);
    g.add_edge(4, 0);
    g.add_edge(4, 1);
    g.add_edge(2, 3);
    g.add_edge(3, 1);
    g.add_edge(3, 5);

    std::vector<int> res;
    bool valid = g.topological_sort(res);
    std::cout << "valid:" << valid << std::endl;

    for (auto e : res) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}
