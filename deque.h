//
// Created by vitalya on 24.06.18.
//

#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#include <iterator>
#include <cstring>

static size_t DEFAULT_CAPACITY = 10;

template <typename T>
class deque {
public:
    template <typename U>
    class iterator_impl;

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
        explicit iterator_impl(iterator_impl<V> const& other);

        iterator_impl& operator=(iterator_impl const& other) = default;


        iterator_impl& operator++();

        iterator_impl operator++(int);

        iterator_impl& operator--();

        iterator_impl operator--(int);


        iterator_impl& operator+=(ptrdiff_t n);

        iterator_impl& operator-=(ptrdiff_t n);

        iterator_impl operator+(ptrdiff_t n);

        iterator_impl operator-(ptrdiff_t n);


        template <typename V>
        bool operator==(iterator_impl<V> const& other) const;

        template <typename V>
        bool operator!=(iterator_impl<V> const& other) const;

        template <typename V>
        bool operator<(iterator_impl<V> const& other) const;

        template <typename V>
        bool operator>(iterator_impl<V> const& other) const;

        template <typename V>
        bool operator<=(iterator_impl<V> const& other) const;

        template <typename V>
        bool operator>=(iterator_impl<V> const& other) const;


        U& operator*() const;

        U* operator->() const;

    private:
        iterator_impl() = default;

        iterator_impl(U* ptr, size_t begin, size_t pos, size_t capacity);

        U* ptr_ = nullptr;
        size_t begin_ = 0;
        size_t pos_ = 0;
        size_t capacity_ = 0;
    };

private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t begin_ = 0;
    size_t end_ = 0;

    void resize() {
        size_t new_capacity = make_capacity(capacity_);
        T* new_data = operator new(new_capacity);
        size_t pos = 0;
        for (size_t i = begin_; i != end_; i = nxt(i), ++pos) {
            new_data[pos] = data_[i];
            data_[i].~T();
        }
        operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
        begin_ = 0;
        end_ = size_;
    }

    size_t make_capacity(size_t capacity) {
        return capacity ? capacity* 2 : DEFAULT_CAPACITY;
    }

    size_t nxt(size_t i) {
        if (i + 1 == capacity_) {
            return 0;
        }
        return i + 1;
    }

    size_t prv(size_t i) {
        if (i == 0) {
            return capacity_ - 1;
        }
        return i - 1;
    }

public:
    deque() = default;

    explicit deque(size_t n);

    deque(deque const& other);

    deque& operator=(deque const& other);

    ~deque();

    void swap(deque& other);


    void push_back(T const& value);

    void push_front(T const& value);

    void pop_back();

    void pop_front();


    iterator begin();

    const_iterator begin() const;

    iterator end();

    const_iterator end() const;

    reverse_iterator rbegin();

    const_reverse_iterator rbegin() const;

    reverse_iterator rend();

    const_reverse_iterator rend() const;


    iterator insert(iterator pos, T const& value);

    iterator erase(iterator pos);


    T& operator[](size_t pos);

    T operator[](size_t pos) const;


    T& front();

    T front() const;

    T& back();

    T back() const;


    bool empty() const;

    void clear();
};

//---------------------------------------------
//--------------------DEQUE--------------------
//---------------------------------------------

template<typename T>
deque<T>::deque(size_t n) : deque() {
    data_ = operator new(sizeof(T) * n);
    capacity_ = n;
}

template<typename T>
deque<T>::deque(deque const &other) :
        size_(other.size_), capacity_(other.capacity_), begin_(other.begin_), end_(other.end_)
{
    data_ = operator new(sizeof(T) * capacity_);
    for (size_t i = begin_; i != end_; i = nxt(i)) {
        data_[i] = other.data_[i];
    }
    //memcpy(data_, other.data_, sizeof(T) * capacity_);
    //std::copy(other.data_, other.data_ + other.capacity_, data_);
}

template<typename T>
deque<T> &deque<T>::operator=(deque const& other) {
    if (&other != this) {
        deque(other).swap(*this);
    }
    return *this;
}

template<typename T>
deque<T>::~deque() {
    for (size_t i = begin_; i != end_; i = nxt(i)) {
        (data_ + i)->~T();
    }
    operator delete(data_);
}

template<typename T>
void deque<T>::swap(deque& other) {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
    swap(begin_, other.begin_);
    swap(end_, other.end_);
}


template<typename T>
void deque<T>::push_back(const T &value) {
    if (size_ == capacity_) {
        resize();
    }
    data_[end_] = value;
    end_ = nxt(end_);
    ++size_;
}

template<typename T>
void deque<T>::push_front(const T &value) {
    if (size_ == capacity_) {
        resize();
    }
    begin_ = prv(begin_);
    data_[begin_] = value;
    ++size_;
}

template<typename T>
void deque<T>::pop_back() {
    end_ = prv(end_);
    data_[end_].~T();
    --size_;
}

template<typename T>
void deque<T>::pop_front() {
    data_[begin_].~T();
    begin_ = nxt(begin_);
    --size_;
}

template<typename T>
typename deque<T>::iterator deque<T>::begin() {
    return iterator_impl<T*>(data_ + begin_, begin_, begin_, capacity_);
}

template<typename T>
typename deque<T>::const_iterator deque<T>::begin() const {
    return iterator_impl<T const*>(data_ + begin_, begin_, begin_, capacity_);
}

template<typename T>
typename deque<T>::iterator deque<T>::end() {
    return iterator_impl<T*>(data_ + end_, end_, end_, capacity_);
}

template<typename T>
typename deque<T>::const_iterator deque<T>::end() const {
    return iterator_impl<T const*>(data_ + end_, end_, end_, capacity_);
}

template<typename T>
typename deque<T>::reverse_iterator deque<T>::rbegin() {
    return std::reverse_iterator(begin());
}

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::rbegin() const {
    return std::reverse_iterator(begin());
}

template<typename T>
typename deque<T>::reverse_iterator deque<T>::rend() {
    return std::reverse_iterator(end());
}

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::rend() const {
    return std::reverse_iterator(end());
}

//---------------------------------------------
//------------------ITERATOR-------------------
//---------------------------------------------

template<typename T>
template<typename U>
template<typename V>
deque<T>::iterator_impl<U>::iterator_impl(const deque::iterator_impl<V> &other) :
        ptr_(other.ptr_), begin_(other.begin_), pos_(other.pos_), capacity_(other.capacity_)
{}


template<typename T>
template<typename U>
deque<T>::iterator_impl<U>::iterator_impl(U* ptr, size_t begin, size_t pos, size_t capacity) :
        ptr_(ptr), begin_(begin), pos_(pos), capacity_(capacity)
{}


template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U>& deque<T>::iterator_impl<U>::operator++() {
    *this += 1;
    return *this;
}

template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U> deque<T>::iterator_impl<U>::operator++(int) {
    iterator_impl tmp(*this);
    *this += 1;
    return tmp;
}

template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U>& deque<T>::iterator_impl<U>::operator--() {
    *this -= 1;
    return *this;
}

template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U> deque<T>::iterator_impl<U>::operator--(int) {
    iterator_impl tmp(*this);
    *this -= 1;
    return tmp;
}


template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U>& deque<T>::iterator_impl<U>::operator+=(ptrdiff_t n) {
    if ((ptrdiff_t )pos_ + n < 0) {
        pos_ += capacity_;
        ptr_ += capacity_;
    }
    pos_ += n;
    ptr_ += n;
    if (pos_ + n >= capacity_) {
        pos_ -= capacity_;
        ptr_ -= capacity_;
    }
    return *this;
}

template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U>& deque<T>::iterator_impl<U>::operator-=(ptrdiff_t n) {
    *this += -n;
    return *this;
}

template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U> deque<T>::iterator_impl<U>::operator+(ptrdiff_t n) {
    iterator_impl tmp(*this);
    return tmp += n;
}

template<typename T>
template<typename U>
typename deque<T>::template iterator_impl<U> deque<T>::iterator_impl<U>::operator-(ptrdiff_t n) {
    iterator_impl tmp(*this);
    return tmp -= n;
}


template<typename T>
template<typename U>
template<typename V>
bool deque<T>::iterator_impl<U>::operator==(const deque::iterator_impl<V> &other) const {
    return ptr_ == other.ptr_;
}

template<typename T>
template<typename U>
template<typename V>
bool deque<T>::iterator_impl<U>::operator!=(const deque::iterator_impl<V> &other) const {
    return ptr_ != other.ptr_;
}

template<typename T>
template<typename U>
template<typename V>
bool deque<T>::iterator_impl<U>::operator<(const deque::iterator_impl<V> &other) const {
    if ((this->pos_ >= this->begin_) ^ (other.pos_ >= other.begin_)) {
        return this->pos_ < other.pos_;
    }
    return this->pos_ > other.pos_;
}

template<typename T>
template<typename U>
template<typename V>
bool deque<T>::iterator_impl<U>::operator<=(const deque::iterator_impl<V> &other) const {
    return !(*this > other);
}

template<typename T>
template<typename U>
template<typename V>
bool deque<T>::iterator_impl<U>::operator>(const deque::iterator_impl<V> &other) const {
    return other < *this;
}

template<typename T>
template<typename U>
template<typename V>
bool deque<T>::iterator_impl<U>::operator>=(const deque::iterator_impl<V> &other) const {
    return !(*this < other);
}


template<typename T>
template<typename U>
U& deque<T>::iterator_impl<U>::operator*() const {
    return *ptr_;
}

template<typename T>
template<typename U>
U* deque<T>::iterator_impl<U>::operator->() const {
    return ptr_;
}

#endif //DEQUE_DEQUE_H
