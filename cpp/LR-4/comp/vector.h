#ifndef VECTOR_H
#define VECTOR_H

using size_t = unsigned long long;
using ptrdiff_t = long long;

struct IteratorBase {};

template <typename T> struct remove_reference      { using type = T; };
template <typename T> struct remove_reference<T&>  { using type = T; };
template <typename T> struct remove_reference<T&&> { using type = T; };

template <typename T>
constexpr T&& forward(typename remove_reference<T>::type& arg) noexcept {
    return static_cast<T&&>(arg);
}

template <typename T>
constexpr T&& forward(typename remove_reference<T>::type&& arg) noexcept {
    return static_cast<T&&>(arg);
}

template <typename T>
constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}


template <typename ValueType>
class Iterator : public IteratorBase {
public:
    using value_type      = ValueType;
    using pointer         = ValueType*;
    using reference       = ValueType&;
    using difference_type = ptrdiff_t;

    Iterator() : ptr(nullptr) {}
    explicit Iterator(pointer p) : ptr(p) {}

    reference operator*() const { return *ptr; }
    pointer   operator->() const { return ptr; }

    Iterator& operator++() { ++ptr; return *this; }
    Iterator  operator++(int) { Iterator tmp(*this); ++ptr; return tmp; }
    Iterator& operator--() { --ptr; return *this; }
    Iterator  operator--(int) { Iterator tmp(*this); --ptr; return tmp; }

    Iterator& operator+=(difference_type n) { ptr += n; return *this; }
    Iterator& operator-=(difference_type n) { ptr -= n; return *this; }
    Iterator  operator+(difference_type n) const { return Iterator(ptr + n); }
    Iterator  operator-(difference_type n) const { return Iterator(ptr - n); }
    friend Iterator operator+(difference_type n, const Iterator& it) {
        return Iterator(it.ptr + n);
    }
    difference_type operator-(const Iterator& other) const {
        return ptr - other.ptr;
    }

    reference operator[](difference_type n) const { return ptr[n]; }

    friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }
    friend bool operator<(const Iterator& a, const Iterator& b)  { return a.ptr < b.ptr; }
    friend bool operator>(const Iterator& a, const Iterator& b)  { return a.ptr > b.ptr; }
    friend bool operator<=(const Iterator& a, const Iterator& b) { return a.ptr <= b.ptr; }
    friend bool operator>=(const Iterator& a, const Iterator& b) { return a.ptr >= b.ptr; }

    pointer base() const { return ptr; }

private:
    pointer ptr;
};

template <typename Iter>
class ReverseIterator : public IteratorBase {
public:
    using value_type      = typename Iter::value_type;
    using pointer         = typename Iter::pointer;
    using reference       = typename Iter::reference;
    using difference_type = ptrdiff_t;

    ReverseIterator() : cur() {}
    explicit ReverseIterator(Iter it) : cur(it) {}

    reference operator*() const { Iter tmp = cur; --tmp; return *tmp; }
    pointer   operator->() const { return &(operator*()); }

    ReverseIterator& operator++() { --cur; return *this; }
    ReverseIterator  operator++(int) { ReverseIterator tmp(*this); --cur; return tmp; }
    ReverseIterator& operator--() { ++cur; return *this; }
    ReverseIterator  operator--(int) { ReverseIterator tmp(*this); ++cur; return tmp; }

    friend bool operator==(const ReverseIterator& a, const ReverseIterator& b) { return a.cur == b.cur; }
    friend bool operator!=(const ReverseIterator& a, const ReverseIterator& b) { return a.cur != b.cur; }

    Iter base() const { return cur; }

private:
    Iter cur;
};

template <typename T>
class Vector {
public:
    using value_type      = T;
    using size_type       = size_t;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;

    using iterator               = Iterator<T>;
    using const_iterator         = Iterator<const T>;
    using reverse_iterator       = ReverseIterator<iterator>;
    using const_reverse_iterator = ReverseIterator<const_iterator>;

    Vector() noexcept : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(size_type count) : Vector() {
        resize(count);
    }

    Vector(const Vector& other) : Vector() {
        reserve(other.size_);
        for (size_type i = 0; i < other.size_; ++i)
            push_back(other.data_[i]);
    }

    Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Vector() {
        destroy_data();
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            destroy_data();
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    void assign(size_type count, const T& value) {
        clear();
        reserve(count);
        for (size_type i = 0; i < count; ++i)
            push_back(value);
    }

    template <typename InputIt>
    void assign(InputIt first, InputIt last) {
        clear();
        while (first != last) {
            push_back(*first);
            ++first;
        }
    }

    reference at(size_type pos) {
        if (pos >= size_) throw "out_of_range";
        return data_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= size_) throw "out_of_range";
        return data_[pos];
    }

    reference operator[](size_type pos) noexcept {
        return data_[pos];
    }
    const_reference operator[](size_type pos) const noexcept {
        return data_[pos];
    }

    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    iterator begin() noexcept { return iterator(data_); }
    iterator end() noexcept { return iterator(data_ + size_); }
    const_iterator begin() const noexcept { return const_iterator(data_); }
    const_iterator end() const noexcept { return const_iterator(data_ + size_); }
    const_iterator cbegin() const noexcept { return const_iterator(data_); }
    const_iterator cend() const noexcept { return const_iterator(data_ + size_); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    size_type capacity() const noexcept { return capacity_; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1) / sizeof(T); }
    bool empty() const noexcept { return size_ == 0; }

    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            T* new_data = static_cast<T*>(operator new(new_cap * sizeof(T)));
            for (size_type i = 0; i < size_; ++i) {
                new (new_data + i) T(move(data_[i]));
                data_[i].~T();
            }
            operator delete(data_);
            data_ = new_data;
            capacity_ = new_cap;
        }
    }

    void resize(size_type count) {
        resize(count, T());
    }

    void resize(size_type count, const T& value) {
        if (count < size_) {
            for (size_type i = count; i < size_; ++i)
                data_[i].~T();
            size_ = count;
        } else if (count > size_) {
            reserve(count);
            for (size_type i = size_; i < count; ++i)
                new (data_ + i) T(value);
            size_ = count;
        }
    }

    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i)
            data_[i].~T();
        size_ = 0;
    }

    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_type index = pos.base() - data_;
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        for (size_type i = size_; i > index; --i) {
            new (data_ + i) T(move(data_[i - 1]));
            data_[i - 1].~T();
        }
        new (data_ + index) T(forward<Args>(args)...);
        ++size_;
        return iterator(data_ + index);
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        new (data_ + size_) T(forward<Args>(args)...);
        ++size_;
        return data_[size_ - 1];
    }

    void push_back(const T& value) {
        emplace_back(value);
    }

    void push_back(T&& value) {
        emplace_back(move(value));
    }

    void pop_back() {
        data_[size_ - 1].~T();
        --size_;
    }

    iterator insert(const_iterator pos, const T& value) {
        size_type index = pos.base() - data_;
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        for (size_type i = size_; i > index; --i) {
            new (data_ + i) T(move(data_[i - 1]));
            data_[i - 1].~T();
        }
        new (data_ + index) T(value);
        ++size_;
        return iterator(data_ + index);
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        size_type index = pos.base() - data_;
        if (count == 0) return iterator(data_ + index);
        if (size_ + count > capacity_)
            reserve((capacity_ + count) > capacity_ * 2 ? capacity_ + count : capacity_ * 2);
        for (size_type i = size_; i > index; --i) {
            new (data_ + i + count - 1) T(move(data_[i - 1]));
            data_[i - 1].~T();
        }
        for (size_type i = 0; i < count; ++i)
            new (data_ + index + i) T(value);
        size_ += count;
        return iterator(data_ + index);
    }

    iterator erase(const_iterator pos) {
        size_type index = pos.base() - data_;
        data_[index].~T();
        for (size_type i = index + 1; i < size_; ++i) {
            new (data_ + i - 1) T(move(data_[i]));
            data_[i].~T();
        }
        --size_;
        return iterator(data_ + index);
    }

    iterator erase(const_iterator first, const_iterator last) {
        size_type idx_first = first.base() - data_;
        size_type idx_last  = last.base() - data_;
        if (idx_first == idx_last) return iterator(data_ + idx_first);
        for (size_type i = idx_first; i < idx_last; ++i)
            data_[i].~T();
        size_type diff = idx_last - idx_first;
        for (size_type i = idx_last; i < size_; ++i) {
            new (data_ + i - diff) T(move(data_[i]));
            data_[i].~T();
        }
        size_ -= diff;
        return iterator(data_ + idx_first);
    }

    void swap(Vector& other) noexcept {
        T* tmp_data = data_; data_ = other.data_; other.data_ = tmp_data;
        size_type tmp_size = size_; size_ = other.size_; other.size_ = tmp_size;
        size_type tmp_cap = capacity_; capacity_ = other.capacity_; other.capacity_ = tmp_cap;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void destroy_data() {
        for (size_type i = 0; i < size_; ++i)
            data_[i].~T();
        operator delete(data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
};

#endif