#include <string_view>
#include <sstream>
#include <gtest/gtest.h>
#include "vector.h"

namespace dsa {
namespace {

TEST(VectorTest, DefaultConstruct) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
}

}
} // namespace dsa
