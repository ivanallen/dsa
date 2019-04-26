#include <iostream>
#include <vector>
#include "../include/graph.h"

/*
int test() {
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
*/

int main() {
    Graph<std::string> g;
    std::vector<int> v;
    v.push_back(g.add_vertex("undershorts")); // 0
    v.push_back(g.add_vertex("pants"));       // 1
    v.push_back(g.add_vertex("belt"));        // 2
    v.push_back(g.add_vertex("shirt"));       // 3
    v.push_back(g.add_vertex("tie"));         // 4
    v.push_back(g.add_vertex("jacket"));      // 5
    v.push_back(g.add_vertex("sockets"));     // 6
    v.push_back(g.add_vertex("shoes"));       // 7
    v.push_back(g.add_vertex("watch"));       // 8

    g.for_each([&](int id, const std::string& value, const std::set<int>& adj) {
        std::cout << "<" << id << "," << value << ">" << std::endl;

        for (auto id : adj) {
            std::cout << value << " -> " << g.value(id) << std::endl;
        }
    });

    g.add_edge(v[0], v[1]);
    g.add_edge(v[0], v[7]);
    g.add_edge(v[1], v[2]);
    g.add_edge(v[1], v[7]);
    g.add_edge(v[2], v[5]);
    g.add_edge(v[3], v[2]);
    g.add_edge(v[3], v[4]);
    g.add_edge(v[4], v[5]);
    g.add_edge(v[6], v[7]);

    std::cout << std::endl;
    g.for_each([&](int id, const std::string& value, const std::set<int>& adj) {
        std::cout << "<" << id << "," << value << ">" << std::endl;

        for (auto id : adj) {
            std::cout << value << " -> " << g.value(id) << std::endl;
        }
    });
}
