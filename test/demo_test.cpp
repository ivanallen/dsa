#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
using namespace std;

class DemoTest : public ::testing::Test {
 public:
  void SetUp() {
    std::cout << "SetUp..." << std::endl;
  }

  void Shutdown() {
    std::cout << "Shutdown..." << std::endl;
  }
};

TEST_F(DemoTest, F) {
  for (size_t i = 0; i < 5; ++i) {
    ASSERT_EQ(i, i);
  }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
