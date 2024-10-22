#include <gtest/gtest.h>
#include <iterator>

#include "vector_t.h"

using task::vector_t;

TEST(Vector_t, TestDefaultConstructor) {
  vector_t<int> vector;

  ASSERT_TRUE(vector.empty());
  ASSERT_EQ(vector.size(), 0);
  ASSERT_EQ(vector.capacity(), 0);
}

TEST(Vector_t, TestDefaultInsertedConstructor) {
  constexpr size_t count = 16;
  vector_t<int> vector(count);

  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), count);
  ASSERT_GE(vector.capacity(), count);
}
TEST(Vector_t, TestCopyValueConstructor) {
  constexpr size_t count = 16;
  vector_t<int> vector(count, 1);

  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), count);
  ASSERT_GE(vector.capacity(), count);
}

TEST(Vector_t, TestConstructorFromIterators) {
  int array[] = {1, 2, 3, 4};
  constexpr size_t count = std::size(array);
  vector_t<int> vector(array, array + count);

  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), count);
  ASSERT_GE(vector.capacity(), count);
}

TEST(Vector_t, TestConstructorFromInitializerList) {
  vector_t vector = {1, 2, 3, 4};

  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 4);
  ASSERT_GE(vector.capacity(), 4);
}

TEST(Vector_t, TestCopyConstructorAndAssignOperator) {
  vector_t vector{1, 2, 3, 4};
  vector_t copy1(vector);
  ASSERT_EQ(vector.size(), copy1.size());
  ASSERT_EQ(vector.capacity(), copy1.capacity());

  vector_t copy2 = vector;
  ASSERT_EQ(vector.size(), copy2.size());
  ASSERT_EQ(vector.capacity(), copy2.capacity());
}

TEST(Vector_t, TestMoveConstructorAndAssignOperator) {
  vector_t vector{1, 2, 3, 4};
  size_t size = vector.size();
  size_t capacity = vector.capacity();
  vector_t moved1(std::move(vector));

  ASSERT_EQ(moved1.size(), size);
  ASSERT_EQ(moved1.capacity(), capacity);

  vector_t moved2 = std::move(moved1);
  ASSERT_EQ(moved2.size(), size);
  ASSERT_EQ(moved2.capacity(), capacity);
}
