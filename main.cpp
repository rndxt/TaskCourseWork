#include <gtest/gtest.h>
#include <iterator>

#include "vector_t.h"

using task::vector_t;

// Private copy constructor and copy assign operator
class ForEmplaceTest {
  int i_;
  double d_;

  ForEmplaceTest(const ForEmplaceTest &);

  ForEmplaceTest &operator=(const ForEmplaceTest &);

public:
  ForEmplaceTest(int i = 0, double d = 0.0) : i_(i), d_(d) {}

  ForEmplaceTest(ForEmplaceTest &&other) noexcept : i_(other.i_), d_(other.d_) {
    other.i_ = 0;
    other.d_ = 0.0;
  }

  ForEmplaceTest &operator=(ForEmplaceTest &&other) noexcept {
    i_ = std::exchange(other.i_, 0);
    d_ = std::exchange(other.d_, 0.0);
    return *this;
  }

  int getInt() const { return i_; }

  double getDouble() const { return d_; }
};

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
  vector_t vector(count, 1);

  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), count);
  ASSERT_GE(vector.capacity(), count);
}

TEST(Vector_t, TestConstructorFromIterators) {
  int array[] = {1, 2, 3, 4};
  constexpr size_t count = std::size(array);
  vector_t vector(array, array + count);

  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), count);
  ASSERT_GE(vector.capacity(), count);

  // empty range
  vector_t emptyVector(array, array);
  ASSERT_TRUE(emptyVector.empty());
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

TEST(Vector_t, TestAccessMethods) {
  vector_t vector{1, 2, 3, 4};
  ASSERT_EQ(vector.front(), 1);
  ASSERT_EQ(vector.back(), 4);
  ASSERT_EQ(vector[0], 1);
  ASSERT_EQ(vector.data(), &vector[0]);
}

TEST(Vector_t, TestPushBack) {
  vector_t<int> vector;
  ASSERT_TRUE(vector.empty());

  vector.push_back(1);
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 1);
  ASSERT_GE(vector.capacity(), 1);
  ASSERT_EQ(vector.front(), 1);

  int x = 2;
  vector.push_back(x);
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 2);
  ASSERT_GE(vector.capacity(), 2);
  ASSERT_EQ(vector.front(), 1);
  ASSERT_EQ(vector.back(), 2);
}

TEST(Vector_t, TestInsert) {
  vector_t<int> vector;
  ASSERT_TRUE(vector.empty());

  vector.insert(vector.end(), 1);
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 1);
  ASSERT_GE(vector.capacity(), 1);
  ASSERT_EQ(vector.front(), 1);

  int x = 2;
  vector.insert(vector.begin(), x);
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 2);
  ASSERT_GE(vector.capacity(), 2);
  ASSERT_EQ(vector.front(), 2);
  ASSERT_EQ(vector.back(), 1);
}

TEST(Vector_t, TestEmplace) {
  vector_t<ForEmplaceTest> vector;
  ASSERT_TRUE(vector.empty());

  vector.emplace(vector.end(), 1, 2.0);
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 1);
  ASSERT_GE(vector.capacity(), 1);
  ASSERT_EQ(vector.front().getInt(), 1);
  ASSERT_EQ(vector.front().getDouble(), 2.0);

  int x = 2;
  vector.emplace(vector.begin(), 2, 3.0);
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 2);
  ASSERT_GE(vector.capacity(), 2);
  ASSERT_EQ(vector.front().getInt(), 2);
  ASSERT_EQ(vector.front().getDouble(), 3.0);
}

TEST(Vector_t, TestErase) {
  vector_t<int> vector;
  ASSERT_TRUE(vector.empty());

  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);
  vector.push_back(4);
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.size(), 4);
  ASSERT_GE(vector.capacity(), 4);

  vector.pop_back();
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.back(), 3);
  ASSERT_EQ(vector.size(), 3);

  vector.erase(vector.begin());
  ASSERT_FALSE(vector.empty());
  ASSERT_EQ(vector.front(), 2);
  ASSERT_EQ(vector.size(), 2);

  vector.pop_back();
  vector.pop_back();
  ASSERT_TRUE(vector.empty());
}
