# Data Structure & Algorithm (DSA)

[![Build Status](https://www.travis-ci.org/ivanallen/dsa.svg?branch=master)](https://travis-ci.com/ivanallen/dsa)

DSA 是一个包含基础和高级数据结构，以及常用算法的库。我们致力于提供一套完整的，简洁的代码实现所有模块。

## Features
- 现代 C++ 风格(基于 C++17 语法)
- 完善的注释
- 接口简单易用
- 强大的 Debug 能力

## How to build
1. 在根目录执行`bash ./build.sh`
2. 在`build/`目录下会生成相关的库和可执行文件

## Example

下面的例子演示如何通过数组创建一棵二叉树。

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

    // them rotate at root
    tree.right_rotate(1);

    // print the tree
    std::cout << tree << std::endl;
    return 0;
}
```

## Contact

- QQ

610441700

- 钉钉群:

<img src="https://github.com/ivanallen/dsa/blob/master/docs/images/DingDingGroup.png"  height="220" width="220">
