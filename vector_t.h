#ifndef TASK_VECTOR_T_H
#define TASK_VECTOR_T_H

#include "buffer_t.h"

namespace task {

template <typename T> class vector_t : private detail::buffer_t<T> {
public:
  using value_type = T;
  using size_type = unsigned;
  using difference_type = int;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;

public:
  vector_t();
  explicit vector_t(size_type count);
  explicit vector_t(size_type count, const_reference value = T());

  template <typename InputIt> vector_t(InputIt first, InputIt last);

  vector_t(const vector_t &other);
  vector_t(vector_t &&other); // noexcept

  vector_t &operator=(const vector_t &other);
  vector_t &operator=(vector_t &&other); // noexcept

  ~vector_t();

public:
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;

  reference front();
  const_reference front() const;

  reference back();
  const_reference back() const;

  T *data() noexcept;
  const T *data() const noexcept;

public:
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type capacity() const noexcept;

  void reserve(size_type newCapacity);

public:
  void clear() noexcept;
  iterator insert(const_iterator pos, const_reference value);
  iterator insert(const_iterator pos, T &&value);

  template <typename... Args>
  iterator emplace(const_reference pos, Args &&... args);

  iterator erase(iterator pos);
  const_iterator erase(const_iterator pos);

  void push_back(const_reference value);
  void push_back(T &&svalue);

  template <typename... Args> reference emplace_back(Args &&... args);

  void pop_back();
  void resize(size_type count);

private:
};

} // namespace task

#endif // TASK_VECTOR_T_H
