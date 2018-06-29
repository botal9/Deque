//
// Created by vitalya on 24.06.18.
//

#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#include <iterator>

static size_t DEFAULT_CAPACITY = 10;

template <typename T>
class deque {
public:
    template <typename U>
    class iterator_impl;

    using iterator = iterator_impl<T>;
    using const_iterator = iterator_impl<T const>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    template <typename U>
    class iterator_impl {
    public:
        friend class deque;

        using iterator_category = std::random_access_iterator_tag;
        using value_type = U;
        using difference_type = ptrdiff_t;
        using pointer = U*;
        using reference = U&;

        template <typename V>
        iterator_impl(iterator_impl<V> const& other);

        iterator_impl& operator=(iterator_impl<U> const& other) = default;


        iterator_impl& operator++();

        iterator_impl operator++(int);

        iterator_impl& operator--();

        iterator_impl operator--(int);


        iterator_impl& operator+=(ptrdiff_t n);

        iterator_impl& operator-=(ptrdiff_t n);

        iterator_impl operator+(ptrdiff_t n);

        iterator_impl operator-(ptrdiff_t n);

        template <typename V>
        ptrdiff_t operator-(iterator_impl<V> const& other) const;


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

    void resize();

    bool is_full();

    size_t make_capacity(size_t capacity);

    size_t nxt(size_t i) const;

    size_t prv(size_t i) const;

public:
    deque() = default;

    explicit deque(size_t n);

    deque(deque const& other);

    deque& operator=(deque const& other);

    ~deque();


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


    iterator insert(const_iterator it, T const& value);

    iterator erase(const_iterator it);


    T& operator[](size_t pos);

    T const& operator[](size_t pos) const;


    T& front();

    T const& front() const;

    T& back();

    T const& back() const;


    bool empty() const;

    void clear();

    size_t size() const;

    void swap(deque& other);
};

//---------------------------------------------
//--------------------DEQUE--------------------
//---------------------------------------------

template<typename T>
deque<T>::deque(size_t n) : deque() {
    data_ = static_cast<T*>(operator new(sizeof(T) * n));
    capacity_ = n;
}

template<typename T>
deque<T>::deque(deque const &other) : deque(other.capacity_) {
    for (size_t i = other.begin_, pos = 0; i != other.end_; i = other.nxt(i), ++pos) {
        new (data_ + pos) T(other.data_[i]);
        ++end_, ++size_;
    }
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
    if (is_full()) {
        resize();
    }
    new (data_ + end_) T(value);
    end_ = nxt(end_);
    ++size_;
}

template<typename T>
void deque<T>::push_front(const T &value) {
    if (is_full()) {
        resize();
    }
    new (data_ + prv(begin_)) T(value);
    begin_ = prv(begin_);
    ++size_;
}

template<typename T>
void deque<T>::pop_back() {
    data_[prv(end_)].~T();
    end_ = prv(end_);
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
    return iterator(data_ + begin_, begin_, begin_, capacity_);
}

template<typename T>
typename deque<T>::const_iterator deque<T>::begin() const {
    return const_iterator(data_ + begin_, begin_, begin_, capacity_);
}

template<typename T>
typename deque<T>::iterator deque<T>::end() {
    return iterator(data_ + end_, begin_, end_, capacity_);
}

template<typename T>
typename deque<T>::const_iterator deque<T>::end() const {
    return const_iterator(data_ + end_, begin_, end_, capacity_);
}

template<typename T>
typename deque<T>::reverse_iterator deque<T>::rbegin() {
    return reverse_iterator(end());
}

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::rbegin() const {
    return const_reverse_iterator(end());
}

template<typename T>
typename deque<T>::reverse_iterator deque<T>::rend() {
    return reverse_iterator(begin());
}

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::rend() const {
    return const_reverse_iterator(begin());
}

template<typename T>
typename deque<T>::iterator deque<T>::insert(deque::const_iterator it, const T &value) {
    size_t pos = size_t(it - begin());
    if (is_full()) {
        resize();
    }
    if (pos < size_ - pos) {
        push_front(value);
        for (size_t i = 0; i < pos; ++i) {
            operator[](i) = operator[](nxt(i));
        }
    } else {
        push_back(value);
        for (size_t i = size_ - 1; i > pos; --i) {
            operator[](i) = operator[](prv(i));
        }
    }
    operator[](pos) = value;
    size_t index = pos + begin_ < capacity_ ? pos + begin_ : pos + begin_ - capacity_;
    return iterator(data_ + index, begin_, index, capacity_);
}

template<typename T>
typename deque<T>::iterator deque<T>::erase(deque::const_iterator it) {
    size_t pos = size_t(it - begin());
    if (pos < size_ - pos) {
        for (size_t i = pos; i > 0; --i) {
            operator[](i) = operator[](prv(i));
        }
        pop_front();
    } else {
        for (size_t i = pos; i < size_ - 1; ++i) {
            operator[](i) = operator[](nxt(i));
        }
        pop_back();
    }
    size_t index = pos + begin_ < capacity_ ? pos + begin_ : pos + begin_ - capacity_;
    return iterator(data_ + index, begin_, index, capacity_);
}

template<typename T>
T& deque<T>::operator[](size_t pos) {
    if (begin_ + pos < capacity_) {
        return data_[begin_ + pos];
    }
    return data_[begin_ + pos - capacity_];
}

template<typename T>
T const& deque<T>::operator[](size_t pos) const {
    if (begin_ + pos < capacity_) {
        return data_[begin_ + pos];
    }
    return data_[begin_ + pos - capacity_];
}

template<typename T>
T &deque<T>::front() {
    return data_[begin_];
}

template<typename T>
T const& deque<T>::front() const {
    return data_[begin_];
}

template<typename T>
T &deque<T>::back() {
    return data_[prv(end_)];
}

template<typename T>
T const& deque<T>::back() const {
    return data_[prv(end_)];
}

template<typename T>
bool deque<T>::empty() const {
    return size_== 0;
}

template<typename T>
void deque<T>::clear() {
    for (size_t i = begin_; i != end_; i = nxt(i)) {
        data_[i].~T();
    }
    operator delete(data_);
    data_ = nullptr;
    size_ = begin_ = end_ = capacity_ = 0;
}

template<typename T>
size_t deque<T>::size() const {
    return size_;
}

template <typename T>
void swap(deque<T> &a, deque<T> &b) {
    a.swap(b);
}



template<typename T>
void deque<T>::resize() {
    size_t new_capacity = make_capacity(capacity_);
    T* new_data = static_cast<T*>(operator new(sizeof(T) * new_capacity));
    for (size_t i = begin_, pos = 0; i != end_; i = nxt(i), ++pos) {
        new (new_data + pos) T(data_[i]);
        data_[i].~T();
    }
    operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
    begin_ = 0;
    end_ = size_;
}

template<typename T>
bool deque<T>::is_full() {
    return size_ + 1 >= capacity_;
}

template<typename T>
size_t deque<T>::make_capacity(size_t capacity) {
    return capacity ? capacity * 2 : DEFAULT_CAPACITY;
}

template<typename T>
size_t deque<T>::nxt(size_t i) const {
    if (i + 1 == capacity_) {
        return 0;
    }
    return i + 1;
}

template<typename T>
size_t deque<T>::prv(size_t i) const {
    if (i == 0) {
        return capacity_ - 1;
    }
    return i - 1;
}


//---------------------------------------------
//------------------ITERATOR-------------------
//---------------------------------------------

template<typename T>
template<typename U>
deque<T>::iterator_impl<U>::iterator_impl(U* ptr, size_t begin, size_t pos, size_t capacity) :
        ptr_(ptr), begin_(begin), pos_(pos), capacity_(capacity)
{}

template<typename T>
template<typename U>
template<typename V>
deque<T>::iterator_impl<U>::iterator_impl(const deque::iterator_impl<V> &other) :
        ptr_(other.ptr_), begin_(other.begin_), pos_(other.pos_), capacity_(other.capacity_)
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
    if (pos_ >= capacity_) {
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
ptrdiff_t deque<T>::iterator_impl<U>::operator-(const deque::iterator_impl<V> &other) const {
    if ((pos_ < begin_) == (other.pos_ < other.begin_)) {
        return pos_ - other.pos_;
    }
    if (this->pos_ < other.pos_) {
        return pos_ - other.pos_ + capacity_;
    }
    return this->pos_ - other.pos_ - capacity_;
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
    if ((this->pos_ >= this->begin_) != (other.pos_ >= other.begin_)) {
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
