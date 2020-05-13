#pragma once

#include <atomic>
#include <cstddef>

template <typename T> class ring_buffer_t
{
public:
  explicit ring_buffer_t(size_t capacity)
  {
    m_capacityMask = capacity - 1;
    for(size_t i = 1; i <= sizeof(void*) * 4; i <<= 1)
      m_capacityMask |= m_capacityMask >> i;
    m_capacity = m_capacityMask + 1;

    m_queue = (Node*)new char[sizeof(Node) * m_capacity];
    for(size_t i = 0; i < m_capacity; ++i)
    {
      m_queue[i].tail.store(i, std::memory_order_relaxed);
      m_queue[i].head.store(-1, std::memory_order_relaxed);
    }

    m_tail.store(0, std::memory_order_relaxed);
    m_head.store(0, std::memory_order_relaxed);
  }

  ~ring_buffer_t()
  {
    for(size_t i = m_head; i != m_tail; ++i)
      (&m_queue[i & m_capacityMask].data)->~T();

    delete [] (char*)m_queue;
  }
  
  size_t capacity() const {return m_capacity;}
  
  size_t size() const
  {

    size_t head = m_head.load(std::memory_order_acquire);
    return m_tail.load(std::memory_order_relaxed) - head;
  }
  

  bool empty() const{
    return size() == 0;
  }


  bool push_back(const T& data)
  {
    Node* node;
    size_t tail = m_tail.load(std::memory_order_acquire);
    for(;;)
    {
      node = &m_queue[tail & m_capacityMask];
      if(node->tail.load(std::memory_order_relaxed) != tail)
        return false;
      if((m_tail.compare_exchange_weak(tail, tail + 1, std::memory_order_relaxed)))
        break;
    }
    new (&node->data)T(data);
    node->head.store(tail, std::memory_order_release);
    return true;
  }

  bool pop_front(T& result)
  {
    Node* node;
    size_t head = m_head.load(std::memory_order_acquire);
    for(;;)
    {
      node = &m_queue[head & m_capacityMask];
      if(node->head.load(std::memory_order_relaxed) != head)
        return false;
      if(m_head.compare_exchange_weak(head, head + 1, std::memory_order_relaxed))
        break;
    }
    result = node->data;
    (&node->data)->~T();
    node->tail.store(head + m_capacity, std::memory_order_release);
    return true;
  }

private:
  struct Node
  {
    T data;
    std::atomic<size_t> tail;
    std::atomic<size_t> head;
  };

private:
  size_t m_capacityMask;
  Node* m_queue;
  size_t m_capacity;
  char cacheLinePad1[64];
  std::atomic<size_t> m_tail;
  char cacheLinePad2[64];
  std::atomic<size_t> m_head;
  char cacheLinePad3[64];
};