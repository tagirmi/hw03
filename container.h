#pragma once

#include <iterator>

namespace hw03 {

namespace details {

template<typename T>
struct node {
  template <typename... Args>
  node(Args&&... args)
    : data(std::forward<Args>(args)...)
    , next{nullptr}
  {
  }

  T data;
  node* next;
};

} // details

template<typename T, typename Allocator = std::allocator<T>>
class forward_list
{
public:
  forward_list()
    : m_head{}
  {
  }
  ~forward_list()
  {
    while (m_head) {
      auto p = m_head;
      m_head = m_head->next;
      m_allocator.destroy(p);
      m_allocator.deallocate(p, 1);
    }
  }

  struct NodeIterator : std::iterator<std::forward_iterator_tag, T>
  {
    explicit NodeIterator(details::node<T>* ptr)
      : m_ptr(ptr)
    {
    }

    NodeIterator& operator++()
    {
      if (m_ptr)
        m_ptr = m_ptr->next;

      return *this;
    }

    T& operator*()
    {
      return m_ptr->data;
    }

    bool operator==(NodeIterator& other)
    {
      return m_ptr == other.m_ptr;
    }

    bool operator!=(NodeIterator& other)
    {
      return !(*this == other);
    }

  private:
    details::node<T>* m_ptr;
  };

  using iterator = NodeIterator;
  using const_iterator = const NodeIterator;

  iterator begin()
  {
    return iterator{m_head};
  }

  const_iterator begin() const
  {
    return iterator{m_head};
  }

  iterator end()
  {
    return iterator{nullptr};
  }

  const_iterator end() const
  {
    return iterator{nullptr};
  }

  void push_back(const T& value)
  {
    auto node = m_allocator.allocate(1);
    m_allocator.construct(node, value);

    if (!m_tail)
      m_head = node;
    else
      m_tail->next = node;

    m_tail = node;
    m_tail->next = nullptr;
  }

private:
  details::node<T>* m_head;
  details::node<T>* m_tail;

  using allocator_t = typename Allocator::template rebind<details::node<T>>::other;
  allocator_t m_allocator;
};

} // hw03
