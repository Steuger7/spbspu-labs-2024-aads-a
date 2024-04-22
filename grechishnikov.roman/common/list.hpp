#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <algorithm>
#include <cstddef>
#include "node.hpp"
#include "iterator.hpp"
#include "constIterator.hpp"
#include "iteratorFunctions.hpp"

namespace grechishnikov
{
  template< typename T >
  class List
  {
  public:
    List();
    List(const List< T >&);
    List(std::initializer_list< T >);
    List(size_t, const T&);
    template< class InputIterator >
    List(InputIterator, InputIterator);
    List(List< T >&& other) noexcept;
    ~List();

    List< T >& operator=(const List< T >&);
    List< T >& operator=(List< T >&&) noexcept;

    Iterator< T > begin() noexcept;
    Iterator< T > end() noexcept;
    ConstIterator< T > cbegin() const noexcept;
    ConstIterator< T > cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void clear() noexcept;

    void push_back(const T&);
    void push_front(const T&);
    void pop_back();
    void pop_front();

    void swap(List< T >&) noexcept;

    void assign(size_t, const T&);
    void assign(Iterator< T >, Iterator< T >);
    void assign(std::initializer_list< T >);

    void remove(const T& data);
    template< typename Predicate >
    void remove_if(Predicate);

    Iterator< T > erase(ConstIterator< T >);

    Iterator< T > insert(ConstIterator< T >, const T&);
    Iterator< T > insert(ConstIterator< T >, T&&);
    Iterator< T > insert(ConstIterator< T >, size_t, const T&);
    template< class InputIterator >
    Iterator< T > insert(ConstIterator< T >, InputIterator, InputIterator);
    Iterator< T > insert(ConstIterator< T >, std::initializer_list< T >);

    void splice(ConstIterator< T >, List< T >&);
    void splice(ConstIterator< T >, List< T >&&);
    void splice(ConstIterator< T >, List< T >&, ConstIterator< T >);
    void splice(ConstIterator< T >, List< T >&&, ConstIterator< T >);
    void splice(ConstIterator< T >, List< T >&, ConstIterator< T >, ConstIterator< T >);
    void splice(ConstIterator< T >, List< T >&&, ConstIterator< T >, ConstIterator< T >);

    void reverse() noexcept;

  private:
    void safe_push_back(const T&);

    size_t size_;
    detail::Node< T >* head_;
    detail::Node< T >* tail_;
  };

  template< typename T >
  List< T >::List():
    size_(0),
    head_(nullptr),
    tail_(nullptr)
  {}

  template< typename T >
  List< T >::List(const List< T >& other):
    List(other.cbegin(), other.cend())
  {}

  template< typename T >
  List< T >::List(std::initializer_list< T > ilist):
    List(ilist.begin(), ilist.end())
  {}

  template< typename T >
  List< T >::List(size_t count, const T& value):
    size_(0),
    head_(nullptr),
    tail_(nullptr)
  {
    for (size_t i = 0; i < count; i++)
    {
      safe_push_back(value);
    }
  }

  template< typename T >
  template< class InputIterator >
  List< T >::List(InputIterator first, InputIterator last):
    size_(0),
    head_(nullptr),
    tail_(nullptr)
  {
    while (first != last)
    {
      safe_push_back(*first);
      first++;
    }
  }

  template< typename T >
  List< T >::List(List< T >&& other) noexcept:
    size_(0),
    head_(nullptr),
    tail_(nullptr)
  {
    swap(other);
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    List< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    if (head_ != other.head_)
    {
      clear();
      swap(other);
    }
    return *this;
  }

  template< typename T >
  Iterator< T > List< T >::begin() noexcept
  {
    return Iterator< T >(head_);
  }

  template< typename T >
  Iterator< T > List< T >::end() noexcept
  {
    return Iterator< T >(nullptr);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return ConstIterator< T >(nullptr);
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return !head_;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_back();
    }
  }

  template< typename T >
  void List< T >::push_back(const T& data)
  {
    detail::Node< T >* pushNode = new detail::Node< T >(data, tail_, nullptr);
    if (!head_)
    {
      head_ = pushNode;
      tail_ = pushNode;
    }
    else
    {
      tail_->next_ = pushNode;
      tail_ = pushNode;
    }
    size_++;
  }

  template< typename T >
  void List< T >::push_front(const T& data)
  {
    detail::Node< T >* pushNode = new detail::Node< T >(data, nullptr, head_);
    if (!head_)
    {
      head_ = pushNode;
      tail_ = pushNode;
    }
    else
    {
      head_->prev_ = pushNode;
      head_ = pushNode;
    }
    size_++;
  }

  template< typename T >
  void List< T >::pop_back()
  {
    if (head_ == tail_)
    {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      detail::Node< T >* temp = tail_->prev_;
      delete tail_;
      tail_ = temp;
      tail_->next_ = nullptr;
    }
    size_--;
  }

  template< typename T >
  void List< T >::pop_front()
  {
    if (head_ == tail_)
    {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      detail::Node< T >* temp = head_->next_;
      delete head_;
      head_ = temp;
      head_->prev_ = nullptr;
    }
    size_--;
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& value)
  {
    List< T > temp(count, value);
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(Iterator< T > first, Iterator< T > last)
  {
    List< T > temp(first, last);
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > ilist)
  {
    List< T > temp(ilist);
    swap(temp);
  }

  template< typename T >
  void List< T >::remove(const T& data)
  {
    auto ifEqual = [&data](const T& remData)
    {
      return remData == data;
    };
    remove_if(ifEqual);
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate p)
  {
    auto first = cbegin();
    while (first != cend())
    {
      if (p(*first))
      {
        erase(first);
      }
      first++;
    }
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > iter)
  {
    detail::Node< T >* prevPoi = iter.node_->prev_;
    detail::Node< T >* nextPoi = iter.node_->next_;
    auto ret = Iterator< T >(nextPoi);
    if (!prevPoi)
    {
      pop_front();
      return ret;
    }
    if (!nextPoi)
    {
      pop_back();
      return ret;
    }
    prevPoi->next_ = nextPoi;
    nextPoi->prev_ = prevPoi;
    size_--;
    delete iter.node_;
    return ret;
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > where, const T& value)
  {
    auto mValue = value;
    return insert(where, std::move(mValue));
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > where, T&& value)
  {
    auto nextNode = advance(where, 1);
    if (nextNode == cend())
    {
      push_back(value);
      return Iterator< T >(tail_);
    }
    auto temp = new detail::Node< T > (value, nullptr, nullptr);
    where.node_->next_ = temp;
    nextNode.node_->prev_ = temp;
    temp->prev_ = where.node_;
    temp->next_ = nextNode.node_;
    size_++;
    return Iterator< T >(temp);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > where, size_t count, const T& value)
  {
    List< T > temp(count, value);
    insert(where, temp.cbegin(), temp.cend());
    return Iterator< T >(where.node_);
  }

  template< typename T >
  template< class InputIterator >
  Iterator< T > List< T >::insert(ConstIterator< T > where, InputIterator first, InputIterator last)
  {
    while (first != last)
    {
      insert(where, *first);
      where++;
      first++;
    }
    return Iterator< T >(where.node_);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > where, std::initializer_list< T > ilist)
  {
    insert(where, ilist.begin(), ilist.end());
    return Iterator< T >(where.node_);
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    List< T > temp;
    auto iter = Iterator< T >(tail_);
    while (iter !=  nullptr)
    {
      temp.push_back(*iter);
      iter--;
    }
    swap(temp);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > where, List< T >& other)
  {
    insert(where, other.cbegin(), other.cend());
    other.clear();
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > where, List< T >&& other)
  {
    insert(where, other.cbegin(), other.cend());
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > where, List< T >& other, ConstIterator< T > iter)
  {
    insert(where, *iter);
    other.erase(iter);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > where, List< T >&& other, ConstIterator< T > iter)
  {
    insert(where, *iter);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > where, List< T >& other, ConstIterator< T > first, ConstIterator< T > last)
  {
    insert(where, first, last);
    auto temp = first;
    while (first != last)
    {
      temp = advance(first, 1);
      other.erase(first);
      first = temp;
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > where, List< T >&& other, ConstIterator< T > first, ConstIterator< T > last)
  {
    splice(where, other, first, last);
  }

  template< typename T >
  void List< T >::safe_push_back(const T& data)
  {
    try
    {
      push_back(data);
    }
    catch (...)
    {
      clear();
      throw;
    }
  }
}

#endif