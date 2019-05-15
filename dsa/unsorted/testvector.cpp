#include "vector.h"
#include<algorithm>
#include <iostream>
using namespace std;
int main()
{
    int nums[] = {1,2,3,4,5,6};
    Vector<int> s(nums, nums + 6);
    Vector<int> b(s.begin(), s.end());
    for (Vector<int>::iterator it = b.begin(); it < b.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
    Vector<int>::iterator it = find(b.begin(), b.end(), 4);
    if (it != b.end()) {
        cout << *it << endl;
    } else {
        cout << "not found!" << endl; 
    }
    return 0;
}