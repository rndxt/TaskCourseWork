#ifndef TASK_VECTOR_T_H
#define TASK_VECTOR_T_H

#include "buffer_t.h"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>

namespace task {

template <typename T>
class vector_t : private detail::buffer_t<T> {
  static_assert(
      std::is_nothrow_move_constructible_v<T>,
      "This implementation works only for no_throw_constructible types");

public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;

public:
  vector_t() noexcept {}

  explicit vector_t(size_type count) : base(count) {
    while (size() < count) {
      defaultConstruct(buffer_ + size_);
      size_ += 1;
    }
  }

  vector_t(size_type count, const_reference value) : base(count) {
    while (size() < count) {
      copyConstruct(buffer_ + size_, value);
      size_ += 1;
    }
  }

  template <typename InputIt>
  vector_t(InputIt first, InputIt last) {
    while (first != last) {
      push_back(*first++);
    }
  }

  vector_t(std::initializer_list<T> list)
      : vector_t(list.begin(), list.end()) {}

  vector_t(const vector_t &other) : base(other.size_) {
    while (size() < other.size()) {
      copyConstruct(buffer_ + size_, other.buffer_[size_]);
      size_ += 1;
    }
  }

  vector_t &operator=(const vector_t &other) {
    vector_t<T> tmp(other);
    std::swap(*this, tmp);
    return *this;
  }

  vector_t(vector_t &&other) noexcept = default;
  vector_t &operator=(vector_t &&other) noexcept = default;

  ~vector_t() = default;

public:
  iterator begin() noexcept { return buffer_; }

  iterator end() noexcept { return buffer_ + size_; }

  const_iterator cbegin() const noexcept { return buffer_; }

  const_iterator cend() const noexcept { return buffer_ + size_; }

  reference operator[](size_type pos) { return *std::next(begin(), pos); }

  const_reference operator[](size_type pos) const {
    return *std::next(cbegin(), pos);
  }

  reference front() { return *begin(); }

  const_reference front() const { return *cbegin(); }

  reference back() { return *std::prev(end()); }

  const_reference back() const { return *std::prev(cend()); }

  T *data() noexcept { return buffer_; }

  const T *data() const noexcept { return buffer_; }

public:
  bool empty() const noexcept { return size_ == 0; }

  size_type size() const noexcept { return size_; }

  size_type capacity() const noexcept { return capacity_; }

  void reserve(size_type newCapacity) {
    if (newCapacity <= capacity_)
      return;

    vector_t<T> tmp(base{newCapacity});
    std::move(begin(), end(), tmp.begin());
    tmp.size_ = size_;
    std::swap(*this, tmp);
  }

public:
  iterator insert(const_iterator pos, const_reference value) {
    T copy(value);
    return insert(pos, std::move(copy));
  }

  iterator insert(const_iterator pos, T &&value) {
    iterator r;
    iterator p = begin() + std::distance(cbegin(), pos);
    if (size() < capacity()) {
      size_ += 1;
      std::move_backward(p, end(), end());
      r = p;
    } else {
      assert(size() == capacity());
      vector_t<T> tmp;
      tmp.reserve(calcNewCapasity());
      tmp.size_ = size_ + 1;
      r = std::move(begin(), p, tmp.begin());
      std::move_backward(p, end(), tmp.end());
      std::swap(*this, tmp);
    }
    *r = std::move(value);
    return r;
  }

  template <typename... Args>
  iterator emplace(const_iterator pos, Args &&...args) {
    T value(std::forward<Args>(args)...);
    return insert(pos, std::move(value));
  }

  template <typename... Args>
  reference emplace_back(Args &&...args) {
    assert(size() <= capacity());
    reserve(calcNewCapasity());
    emplaceConstruct(buffer_ + size_, std::forward<Args>(args)...);
    ++size_;
    return back();
  }

  void push_back(const_reference value) {
    T tmp(value);
    emplace_back(std::move(tmp));
  }

  void push_back(T &&value) {
    emplace_back(std::move(value));
  }

  void pop_back() {
    assert(!empty());
    size_ -= 1;
    detail::destroy(buffer_ + size_);
  }

  void resize(size_type count) {
    while (size() < count) {
      emplace_back();
    }

    while (size() > count) {
      pop_back();
    }
  }

  iterator erase(iterator pos) {
    assert(pos != end());
    auto i = std::distance(begin(), pos);
    std::rotate(pos, std::next(pos), end());
    pop_back();
    return begin() + i;
  }

  const_iterator erase(const_iterator pos) {
    auto i = std::distance(cbegin(), pos);
    return erase(std::next(begin(), i));
  }

private:
  using base = detail::buffer_t<T>;
  using base::buffer_;
  using base::capacity_;
  using base::size_;

  vector_t(base &&buffer) : base(std::move(buffer)) {}

  template <typename... Args>
  static void emplaceConstruct(T *ptr, Args &&...args) {
    assert(ptr != nullptr);
    new (ptr) T(std::forward<Args>(args)...);
  }

  static void copyConstruct(T *ptr, const T &value) {
    emplaceConstruct(ptr, value);
  }

  static void defaultConstruct(T *ptr) { emplaceConstruct(ptr); }

  size_type calcNewCapasity() const {
    if (capacity_ == 0)
      return 1;

    if (size() == capacity_)
      return 2 * capacity_;

    return capacity_;
  }
};

template <typename InputIt>
vector_t(InputIt first, InputIt last)
    -> vector_t<typename std::iterator_traits<InputIt>::value_type>;

} // namespace task

#endif // TASK_VECTOR_T_H
