#pragma once

#include <cstdint>
#include <cstdlib>
#include <new>
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
    , m_index{0}
  {
  }

  ~MemoryPool()
  {
    std::free(m_mem);
  }

  void* get_mem(size_t n)
  {
    if (n + m_index <= BlockCount) {
      auto ptr = m_mem + m_index * BlockSize;
      m_index += n;
      return ptr;
    }

    return nullptr;
  }

  void put_mem(void* p, size_t n)
  {
    auto ptr = static_cast<uint8_t*>(p);

    if (ptr < m_mem || ptr >= m_mem + memSize)
      return;

    m_index -= n;
  }

private:
  uint8_t* m_mem;
  size_t m_index;
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
