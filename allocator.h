#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <utility>

namespace hw03 {

namespace details {

template<size_t BlockSize, size_t BlockCount>
class MemoryPool
{
  static constexpr size_t memSize = BlockSize * BlockCount;

public:
  static MemoryPool& instance()
  {
    static MemoryPool pool;
    return pool;
  }

  MemoryPool()
    : m_mem{static_cast<uint8_t*>(std::malloc(memSize))}
    , m_isFree{}
  {
    std::generate_n(m_isFree, BlockCount, []() { return true; });
  }

  ~MemoryPool()
  {
    std::free(m_mem);
  }

  void* get_mem(size_t n)
  {
    if (n > BlockCount)
      return nullptr;

    void* ptr = nullptr;

    for (size_t i = 0; i < BlockCount; ++i) {
      if (isRangeFree(i, n)) {
        ptr = grab(i, n);
        break;
      }
    }

    return ptr;
  }

  void put_mem(void* p, size_t n)
  {
    if (!checkPtr(p))
      throw std::invalid_argument("MemoryPool::put_mem(void*, size_t)");

    const size_t begin = findIndex(p);

    if (!checkRange(begin, n))
      throw std::invalid_argument("MemoryPool::put_mem(void*, size_t)");

    for (size_t i = begin; i < n; ++i)
      m_isFree[i] = true;
  }

private:
  bool isRangeFree(size_t begin, size_t n) const
  {
    for (size_t i = begin; i < n; ++i)
      if (!m_isFree[i])
        return false;

    return true;
  }

  void* grab(size_t begin, size_t n)
  {
    for (size_t i = begin; i < n; ++i)
      m_isFree[i] = false;

    return m_mem + begin + n * BlockSize;
  }

  bool checkPtr(void* ptr) const
  {
    return ptr >= m_mem && ptr < m_mem + memSize;
  }

  size_t findIndex(void* ptr) const
  {
    return  (static_cast<uint8_t*>(ptr) - m_mem) / BlockSize;
  }

  bool checkRange(size_t i, size_t n) const
  {
    return i + n < BlockCount;
  }

  uint8_t* m_mem;
  bool m_isFree[BlockCount];
};

} // details

template<typename T, std::size_t N>
struct allocator {
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

  using pool = details::MemoryPool<sizeof(value_type), N>;

  template<typename U>
  struct rebind {
    using other = allocator<U, N>;
  };

  allocator() = default;
  ~allocator() = default;

  template<typename U>
  allocator(const allocator<U, N>&)
  {
  }

  pointer allocate(std::size_t n)
  {
    auto p = pool::instance().get_mem(n);
    if (!p)
      throw std::bad_alloc();
    return reinterpret_cast<pointer>(p);
  }

  void deallocate(pointer p, std::size_t n)
  {
    pool::instance().put_mem(p, n);
  }

  template<typename U, typename ...Args>
  void construct(U* p, Args&&... args)
  {
    new(p) U(std::forward<Args>(args)...);
  }

  void destroy(pointer p)
  {
    p->~T();
  }
};

} // namespace hw03
