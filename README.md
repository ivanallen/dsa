# Data Structure & Algorithm (DSA)

[![Build Status](https://www.travis-ci.org/ivanallen/dsa.svg?branch=master)](https://travis-ci.com/ivanallen/dsa)

DSA is a library of basic and advanced data structure, as well as common algorithm. We are committed to providing a complete and concise set of code to implement all modules.

## Features

- Modern C++ style(Based on C++17)
- Full comments
- Clear and pleasant API
- Strong ability to debug

## How to build

```
$ bash ./build.sh
$ cd build & make test
```

## Example

A easy demo showing how to create a binary tree.

```c++
#include <iostream>
#include <string_view>
#include "binary_tree.h"

using namespace dsa;
using Tree = BinaryTree<int>;

int main() {
    constexpr std::string_view tree_graph = R"(
                  1        <- right_rotate
                /   \
               2     3
             /   \
            4     5

                  ||

                  2
                /   \
               4     1
                   /   \
                  5     3
    )";

    // We can build a binary tree from list
    Tree tree({1, 2, 3, 4, 5});
    std::cout << tree << std::endl;

    // Rotate at root
    tree.right_rotate(1);

    // Print the tree
    std::cout << tree << std::endl;
    return 0;
}
```

## Contact

- QQ

610441700

- DingDingGroup:

<img src="https://github.com/ivanallen/dsa/blob/master/docs/images/DingDingGroup.png"  height="220" width="220">
