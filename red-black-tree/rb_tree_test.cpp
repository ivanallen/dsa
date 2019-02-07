#include <red-black-tree/rb_tree.h>
#include <string_view>


void show(const RbTree<int, int>& rbt, const char* prompt = nullptr) {
    std::string_view RED("\x1b[31m");
    std::string_view CLEAR("\x1b[0m");
    if (prompt != nullptr)
        std::cout << prompt << ' ';
    std::cout << "inorder:[";
    rbt.inorder([&](const int& key, const int& value, Color color) {
        std::cout << ' ' << (color == Color::RED ? RED : "") << key << CLEAR;
    });
    std::cout << " ] preorder:[";
    rbt.preorder([&](const int& key, const int& value, Color color) {
        std::cout << ' ' << (color == Color::RED ? RED : "") << key << CLEAR;
    });
    std::cout << " ] postorder:[";
    rbt.postorder([&](const int& key, const int& value, Color color) {
        std::cout << ' ' << (color == Color::RED ? RED : "") << key << CLEAR;
    });
    std::cout << " ]" << std::endl;
}

int main() {
    RbTree<int, int> rbt;

    /*
    for (int i = 50; i >= 36; --i) {
        rbt.insert(i, i);
    }

    for (int i = 0; i < 20; ++i) {
        rbt.insert(i, i);
    }
    */
    for (int i = 5; i >= 1; --i) {
        rbt.insert(i, i);
    }

    for (int i = 6; i <= 10; ++i) {
        rbt.insert(i, i);
    }

    show(rbt);

    return 0;
}
