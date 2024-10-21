#ifndef DETAIL_BUFFER_T_H
#define DETAIL_BUFFER_T_H

#include <utility>

namespace task {

namespace detail {

template <typename T> class buffer_t {
protected:
  T *buffer_;
  unsigned size_;
  unsigned capasity_;

protected:
  buffer_t(unsigned count = 0)
      : buffer_(alloc_memory(count)), size_(0), capasity_(count) {}

  buffer_t(const buffer_t &other) = delete;
  buffer_t &operator=(const buffer_t &other) = delete;

  buffer_t(buffer_t &&other) noexcept
      : buffer_(std::exchange(other.buffer_, nullptr)),
        size_(std::exchange(other.size_, nullptr)),
        capasity_(std::exchange(other.capasity_, nullptr)) {}

  buffer_t &operator=(buffer_t &&other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(size_, other.size_);
    std::swap(capasity_, other.capasity_);
    return *this;
  }

  ~buffer_t() {
    destroy(buffer_, buffer_ + size_);
    ::operator delete(buffer_);
  }

  void destroy(T *ptr) { ptr->~T(); }

  template <typename ForwardIt> void destroy(ForwardIt begin, ForwardIt end) {
    while (begin != end) {
      destroy(&*begin++);
    }
  }

private:
  T *alloc_memory(size_t count) {
    if (count == 0)
      return nullptr;
    return static_cast<T *>(::operator new(count * sizeof(T)));
  }
};

} // namespace detail

} // namespace task

#endif // DETAIL_BUFFER_T_H
