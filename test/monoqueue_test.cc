#include "../include/monoqueue.h"
#include <iostream>
#include <functional>

int main() {
    MonoQueue<int, std::greater<int>> mq;

    mq.push(1);
    mq.push(5);
    mq.push(2);
    mq.push(2);
    mq.push(2);
    mq.push(6);
    mq.push(5);
    mq.push(3);
    mq.push(7);
    mq.push(4);
    mq.push(8);
    mq.push(4);
    mq.push(2);
    mq.push(6);

    while(!mq.empty()) {
        std::cout << mq.front() << ' ';
        mq.pop();
    }
    std::cout << std::endl;
    return 0;
}
