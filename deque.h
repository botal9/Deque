//
// Created by vitalya on 24.06.18.
//

#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#include <iterator>

template <typename T>
class deque {
public:
    template <typename U> class iterator_impl;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;
    typedef iterator_impl<T*> iterator;
    typedef iterator_impl<T const*> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    template <typename U>
    class iterator_impl :
        public std::iterator<
                iterator_category,
                value_type,
                difference_type,
                pointer,
                reference
        >
    {
    public:
        friend class deque;


        template <typename V>
        iterator_impl(iterator_impl<V> const& other);

        iterator_impl& operator=(iterator_impl const& other) = default;


        iterator_impl& operator++();

        iterator_impl operator++(int);

        iterator_impl& operator--();

        iterator_impl operator--(int);


        template <typename V>
        bool operator==(iterator_impl<V> const& other) const;

        template <typename V>
        bool operator!=(iterator_impl<V> const& other) const;


        U& operator*() const;

        U* operator->() const;

    private:
        iterator_impl();

        U* ptr;
        size_t begin_;
    };

private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t bigin_ = 0;
    size_t end_ = 0;

    void resize(size_t capacity);

public:
    deque() = default;

    explicit deque(size_t size);

    deque(deque const& other);

    deque& operator=(deque const& other);

    ~deque();

    void swap(deque other);


    void push_back(T const& value);

    void push_front(T const& value);

    void pop_back();

    void pop_front();


    iterator insert(iterator pos, T const& value);

    iterator erase(iterator pos);


    T& operator[](size_t pos);

    T operator[](size_t pos) const;


    T& front();

    T front() const;

    T& back();

    T back() const;


    

};

#endif //DEQUE_DEQUE_H
