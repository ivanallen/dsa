#pragma once

#include <vector>

class UnionFind {
public:
    UnionFind(int n) : _id(n), _weight(n, 1), _count(n) {
        for (int i = 0; i < n; ++i) {
            _id[i] = i;
        }
    }

    bool connect(int a, int b) {
        int r1 = _root(a);
        int r2 = _root(b);
        if (r1 == r2) return false;
        if (_weight[r1] > _weight[r2]) {
            _id[r2] = r1;
            _weight[r1] += _weight[r2];
        } else {
            _id[r1] = r2;
            _weight[r2] += _weight[r1];
        }
        --_count;
        return true;
    }

    bool connected(int a, int b) const {
        int r1 = _root(a);
        int r2 = _root(b);
        return r1 == r2;
    }

    int count() const {
        return _count;
    }

private:
    int _root(int a) const {
        while (a != _id[a]) {
            _id[a] = _id[_id[a]]; // path compress
            a = _id[a];
        }
        return a;
    }
private:
    mutable std::vector<int> _id;
    std::vector<int> _weight;
    int _count;
};
