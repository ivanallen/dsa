#include <iostream>
#include <string>
#include <vector>

std::vector<int> radius(const std::string& p, int* center) {
    int e = 0; // right edge
    int c = 0; // center
    int maxRadius = 0;

    std::vector<int> r(p.length()); // radius
    for (int i = 0; i < p.length(); ++i) {
        r[i] = i < e ? std::min(r[2*c-i], e - i) : 1;
        while (0 <= i-r[i] && i + r[i] < p.length() && p[i-r[i]] == p[i+r[i]]) {
            ++r[i];
        }

        if (i + r[i] > e) {
            c = i;
            e = i + r[i];
        }

        if (r[i] > maxRadius) {
            *center = c;
            maxRadius = r[i];
        }
    }

    return r;
}

std::string manacher(const std::string& str) {
    std::string p = "$#";
    for (auto c : str) {
        p += c;
        p += '#';
    }
    int center = 0;
    auto r = radius(p, &center);
    std::cout << "center:" << center << " radius:" << r[center] << std::endl;
    std::cout << "start:" << (center-r[center])/2 << " length:" << r[center]-1 << std::endl;
    return str.substr((center - r[center])/2, r[center]-1);
}

int main() {
    int center = 0;
    std::string p;
    std::cin >> p;

    std::cout << "p: " << p << std::endl;
    std::cout << "longest palindrome: " << manacher(p) << std::endl;
    return 0;
}
