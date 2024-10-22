#ifndef DETAIL_BUFFER_T_H
#define DETAIL_BUFFER_T_H

#include <utility>

namespace task {

namespace detail {

template <typename T>
void destroy(T *ptr) { ptr->~T(); }

template <typename ForwardIt> void destroy(ForwardIt begin, ForwardIt end) {
  while (begin != end) {
    destroy(&*begin++);
  }
}

template <typename T> class buffer_t {
protected:
  T *buffer_ = nullptr;
  std::size_t size_ = 0;
  std::size_t capacity_ = 0;

protected:
  buffer_t() = default;

  buffer_t(std::size_t count)
      : buffer_(alloc_memory(count)), capacity_(count) {}

  buffer_t(const buffer_t &other) = delete;
  buffer_t &operator=(const buffer_t &other) = delete;

  buffer_t(buffer_t &&other) noexcept
      : buffer_(std::exchange(other.buffer_, nullptr)),
        size_(std::exchange(other.size_, 0)),
        capacity_(std::exchange(other.capacity_, 0)) {}

  buffer_t &operator=(buffer_t &&other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    return *this;
  }

  ~buffer_t() {
    destroy(buffer_, buffer_ + size_);
    ::operator delete(buffer_);
  }

private:
  T *alloc_memory(std::size_t count) {
    if (count == 0)
      return nullptr;
    return static_cast<T *>(::operator new(count * sizeof(T)));
  }
};

} // namespace detail

} // namespace task

#endif // DETAIL_BUFFER_T_H
