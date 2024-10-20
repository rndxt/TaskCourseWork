#include <gtest/gtest.h>

#include "vector_t.h"

TEST(Vector_t, TestDefaultConstructor) {
  task::vector_t<int> vector;
  ASSERT_TRUE(vector.empty());
  ASSERT_EQ(vector.size(), 0);
  ASSERT_EQ(vector.capacity(), 0);
}
