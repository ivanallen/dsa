#include <string>
#include <string.h>
#include <memory>
#include <iostream>
#include <vector>

namespace kmp {

	void preprocess(const char* needle, int* b) {
		b[0] = -1;
		int i = 0;
		int j = -1;
		while (needle[i]) {
			while (j != -1 && needle[i] != needle[j]) j = b[j];
			i++;
			j++;
			b[i] = j;
		}
	}

	void preprocess(const std::string needle, int* b)
	{
		b[0] = -1;
		int i = 0;
		int j = -1;
		for (auto iter = needle.begin(); iter < needle.end(); iter++) {
			while (j != -1 && needle[i] != needle[j]) j = b[j];
			i++;
			j++;
			b[i] = j;
		}
	}

	template<typename RandomIt>
	void preprocess(RandomIt needleBegin, RandomIt needleEnd, int* b)
	{
		b[0] = -1;
		int i = 0;
		int j = -1;
		for (auto iter = needleBegin; iter < needleEnd; iter++) {
			while (j != -1 && *(needleBegin + i) != *(needleBegin+j)) j = b[j];
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

	int find(const std::string haystack, const std::string needle)
	{
		int n = haystack.size();
		int m = needle.size();
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

	template<typename RandomIt>
	int find(RandomIt haystackBegin, RandomIt haystackEnd, RandomIt needleBegin, RandomIt needleEnd)
	{
		int n = haystackEnd - haystackBegin;
		int m = needleEnd - needleBegin;
		auto b = std::make_unique<int[]>(m + 1);
		preprocess(needleBegin, needleEnd, b.get());
		for (int i = 0; i < m + 1; ++i) {
			std::cout << b[i] << " ";
		}
		std::cout << std::endl;

		int i = 0;
		int j = 0;
		while (i < n) {
			while (j != -1 && *(haystackBegin + i) != *(needleBegin + j)) j = b[j];
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
	std::string haystackStr = "xxxxxababababxyz";
	std::string needleStr = "abababx";
	std::vector<int> haystackVec = { 24,24,24,24,24,1,2,1,2,1,2,1,2,24,25,26 };
	std::vector<int> needleVec = { 1,2,1,2,1,2,24 };
	std::cout << kmp::find(haystack, needle) << std::endl;
	std::cout << kmp::find(haystackStr, needleStr) << std::endl;
	std::cout << kmp::find(haystackVec.begin(), haystackVec.end(), needleVec.begin(), needleVec.end());
	return 0;
}