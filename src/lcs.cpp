#include <iostream>
#include <vector>

int long_common_substring(const std::string& s1, const std::string& s2) {
    int m = s1.length();
    int n = s2.length();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    int result = 0;
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = 1 + dp[i-1][j-1];
            } else {
                dp[i][j] = 0;
            }
            result = dp[i][j] > result ? dp[i][j] : result;
        }
    }
    return result;
}

int long_common_subsequence(const std::string& s1, const std::string& s2) {
    int m = s1.length();
    int n = s2.length();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = 1 + dp[i-1][j-1];
            } else {
                dp[i][j] = std::max(dp[i-1][j-1], std::max(dp[i][j-1], dp[i-1][j]));
            }
        }
    }
    return dp[m][n];
}

void test1() {
    std::string s1;
    std::string s2;
    std::cin >> s1 >> s2;
    std::cout << s1 << '\n' << s2 << std::endl;
    std::cout << "lcs(substr):" << long_common_substring(s1, s2) << std::endl;
    std::cout << "lcs(subseq):" << long_common_subsequence(s1, s2) << std::endl;
}

int main() {
    test1();
    return 0;
}
