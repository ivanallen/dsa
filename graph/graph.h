#pragma once
#include <map>
#include <queue>
#include <set>


template <typename T>
class Graph {
public:
    class Vertex;
    // 暂时只能添加，因此 size 是递增的，可以充当 id
    template <typename U>
    int add_vertex(U&& value) {
        Vertex v(std::forward<U>(value), _size++);
        int id = v.id();
        _adjcent.emplace(id, std::set<int>{});
        _vertexes.emplace(id, std::move(v));
        return id;
    }

    bool add_edge(int u, int v) {
        auto a = _vertexes.find(u);
        if (a == _vertexes.end()) return false;
        auto b = _vertexes.find(v);
        if (b == _vertexes.end()) return false;
        _adjcent[u].insert(v);;
        return true;
    }

    T& value(int id) {
        auto it = _vertexes.find(id);
        if (it != _vertexes.end()) {
            return it->second.value();
        }
        throw std::runtime_error("no such vertex `" + std::to_string(id) + "`");
    }

    template <typename FUNC>
    void for_each(FUNC&& cb) {
        for (auto& pair : _vertexes) {
            cb(pair.first, pair.second.value(), _adjcent[pair.first]);
        }
    }

    void BFS() {
    }
private:

private:
    std::map<int, Vertex> _vertexes;
    std::map<int, std::set<int>> _adjcent;
    int _size = 0;
};

template <typename T>
class Graph<T>::Vertex {
public:
    friend class Graph;
    enum Color {
        WHITE = 0,
        GRAY,
        BLACK
    };

    T& value() {
        return *_value;
    }
private:
    template <typename U>
    Vertex(U&& value, int id) : _id(id) {
        _value.reset(new T(std::forward<U>(value)));
    }

    int id() const {
        return _id;
    }
private:
    std::unique_ptr<T> _value;
    int _id = 0;
    Vertex* _parent = nullptr;
    int _depth = 0;
    Color _color = Color::WHITE;
};

/*
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
*/
