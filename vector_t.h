#ifndef TASK_VECTOR_T_H
#define TASK_VECTOR_T_H

#include "buffer_t.h"

#include <cassert>
#include <initializer_list>
#include <iterator>

namespace task {

template <typename T> class vector_t : public detail::buffer_t<T> {
private:
  using base = detail::buffer_t<T>;
  using base::buffer_;
  using base::capacity_;
  using base::size_;

  static void defaultConstruct(T *ptr) { new (ptr) T(); }

  static void copyConstruct(T *ptr, const T &value) { new (ptr) T(value); }

public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
  vector_t() noexcept {}

  explicit vector_t(size_type count) noexcept(
      std::is_nothrow_default_constructible_v<T>)
      : base(count) {
    while (size_ < count) {
      defaultConstruct(buffer_ + size_);
      size_ += 1;
    }
  }

  vector_t(size_type count, const_reference value) : base(count) {
    while (size_ < count) {
      copyConstruct(buffer_ + size_, value);
      size_ += 1;
    }
  }

  template <typename InputIt> vector_t(InputIt first, InputIt last) {
    while (first != last) {
      push_back(*first++);
    }
  }

  vector_t(std::initializer_list<T> list)
      : vector_t(list.begin(), list.end()) {}

  vector_t(const vector_t &other) : base(other.size_) {
    while (size_ < other.size_) {
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

    vector_t<T> tmp(newCapacity);
    for (size_type i = 0; i != size_; ++i) {
      tmp.buffer_[i] = std::move(buffer_[i]);
    }
    tmp.size_ = size_;
    std::swap(*this, tmp);
  }

public:
  iterator insert(const_iterator pos, const_reference value);
  iterator insert(const_iterator pos, T &&value);

  template <typename... Args>
  iterator emplace(const_reference pos, Args &&...args);

  template <typename... Args> reference emplace_back(Args &&...args);

  void push_back(const_reference value) {
    T tmp(value);
    push_back(std::move(tmp));
  }

  void push_back(T &&value) {
    assert(size_ <= capacity_);
    static_assert(std::is_nothrow_move_constructible_v<T>);
    if (capacity_ == 0) {
      reserve(1);
    } else if (size_ == capacity_) {
      reserve(2 * size_);
    }

    buffer_[size_] = std::move(value);
    ++size_;
  }

  void pop_back() {
    assert(size_ > 0);
    size_ -= 1;
    detail::destroy(buffer_ + size_);
  }

  void resize(size_type count);

  iterator erase(iterator pos);
  const_iterator erase(const_iterator pos);
};

template <class InputIt>
vector_t(InputIt first, InputIt last)
    -> vector_t<typename std::iterator_traits<InputIt>::value_type>;

} // namespace task

#endif // TASK_VECTOR_T_H
