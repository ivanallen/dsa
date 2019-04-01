#pragma once
#include <map>
#include <queue>

class Graph {
public:
    void add_edge(int a, int b) {
        if (_v.find(a) == _v.end()) {
            _v[a] = 0;
        }
        ++_v[b];
        _adj[a].push_back(b);
    }

    bool topological_sort(std::vector<int>& res) {
        auto indegree = _v;
        std::queue<int> q;
        for (auto& kv : indegree) {
            if (kv.second == 0) {
                q.push(kv.first);
            }
        }

        int count = 0;
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            ++count;
            res.push_back(v);
            for (auto e : _adj[v]) {
                if (--indegree[e] == 0) {
                    q.push(e);
                }
            }
        }
        return count == indegree.size();
    }

private:
    std::map<int, int> _v;
    std::map<int, std::vector<int>> _adj;
};
