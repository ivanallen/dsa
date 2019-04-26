#include <string.h>
#include <memory>
#include <iostream>

namespace kmp {

void preprocess(const char* needle, int* b) {
    b[0] = -1;
    int i = 0;
    int j = -1;
    while (needle[i]) {
        while(j != -1 && needle[i] != needle[j]) j = b[j];
        i++;
        j++;
        b[i] = j;
    }
}

int find(const char* haystack, const char* needle) {
    int n = strlen(haystack);
    int m = strlen(needle);
    auto b = std::make_unique<int[]>(m + 1);
    preprocess(needle, b.get());
    for (int i = 0; i < m + 1; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;

    int i = 0;
    int j = 0;
    while (i < n) {
        while (j != -1 && haystack[i] != needle[j]) j = b[j];
        i++;
        j++;
        if (j == m) return i - j;
        //         0  1  2  3  4  5  6  7
        // b[] = [-1, 0, 0, 1, 2, 3, 4, 0]
        //         a  b  a  b  a  b  x
        // 失配原因2: 字符不匹配
        //            - 
        // xxxxxababababxyz
        //      abababx
        //        abababx  
    }

    return -1;
}

} // namespace kmp

int main() {
    const char* haystack = "xxxxxababababxyz";
    const char* needle = "abababx";
    std::cout << kmp::find(haystack, needle) << std::endl;
    return 0;
}
