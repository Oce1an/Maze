#ifndef DEQUE_H
#define DEQUE_H

#include <cstddef>
#include <stdexcept>

template<typename T>
class Deque {
public:
    static const size_t BLOCK_SIZE = 8;

private:
    T** blocks;         
    size_t block_count;  
    size_t begin_block;
    size_t begin_idx;
    size_t end_block;
    size_t end_idx;
    size_t size_;

    void allocate_block(size_t block_idx) {
        if (!blocks[block_idx])
            blocks[block_idx] = new T[BLOCK_SIZE];
    }

    void ensure_capacity() {
        if (size_ == block_count * BLOCK_SIZE)
            resize_blocks(block_count == 0 ? 1 : block_count * 2);
    }

    void resize_blocks(size_t new_cap) {
        if (new_cap == 0) new_cap = 1;
        T** new_blocks = new T*[new_cap]();
        size_t old_logical = begin_block * BLOCK_SIZE + begin_idx;
        size_t new_size = size_;
        for (size_t i = 0; i < (size_ + BLOCK_SIZE - 1) / BLOCK_SIZE; ++i) {
            size_t old_block = (begin_block + i) % block_count;
            new_blocks[i] = blocks[old_block];
        }
        delete[] blocks;
        blocks = new_blocks;
        block_count = new_cap;
        begin_block = 0;
        begin_idx = 0;

        end_block = size_ / BLOCK_SIZE;
        end_idx = size_ % BLOCK_SIZE;
    }

public:
    Deque() : blocks(nullptr), block_count(0), begin_block(0), begin_idx(0),
              end_block(0), end_idx(0), size_(0) {
        resize_blocks(1); 
    }

    ~Deque() {
        clear();
        for (size_t i = 0; i < block_count; ++i)
            delete[] blocks[i];
        delete[] blocks;
    }

    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    void push_back(const T& value) {
        ensure_capacity();
        if (!blocks[end_block]) allocate_block(end_block);
        blocks[end_block][end_idx] = value;
        ++end_idx;
        if (end_idx == BLOCK_SIZE) {
            end_idx = 0;
            end_block = (end_block + 1) % block_count;
        }
        ++size_;
    }

    void push_front(const T& value) {
        ensure_capacity();
        if (begin_idx == 0) {
            begin_block = (begin_block == 0 ? block_count - 1 : begin_block - 1);
            begin_idx = BLOCK_SIZE - 1;
        } else {
            --begin_idx;
        }
        if (!blocks[begin_block]) allocate_block(begin_block);
        blocks[begin_block][begin_idx] = value;
        ++size_;
    }

    void pop_back() {
        if (size_ == 0) throw std::underflow_error("Deque empty");
        if (end_idx == 0) {
            end_block = (end_block == 0 ? block_count - 1 : end_block - 1);
            end_idx = BLOCK_SIZE - 1;
        } else {
            --end_idx;
        }
        --size_;
    }

    void pop_front() {
        if (size_ == 0) throw std::underflow_error("Deque empty");
        ++begin_idx;
        if (begin_idx == BLOCK_SIZE) {
            begin_idx = 0;
            begin_block = (begin_block + 1) % block_count;
        }
        --size_;
    }

    T& operator[](size_t index) {
        size_t logical = begin_block * BLOCK_SIZE + begin_idx + index;
        size_t b = (logical / BLOCK_SIZE) % block_count;
        size_t i = logical % BLOCK_SIZE;
        return blocks[b][i];
    }

    const T& operator[](size_t index) const {
        size_t logical = begin_block * BLOCK_SIZE + begin_idx + index;
        size_t b = (logical / BLOCK_SIZE) % block_count;
        size_t i = logical % BLOCK_SIZE;
        return blocks[b][i];
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        while (!empty()) pop_back();
    }

    class iterator {
        friend class Deque;
        Deque* deque;
        size_t pos; 
        iterator(Deque* d, size_t p) : deque(d), pos(p) {}
    public:
        T& operator*() const { return (*deque)[pos]; }
        T* operator->() const { return &(*deque)[pos]; }

        iterator& operator++() { ++pos; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++pos; return tmp; }
        iterator& operator--() { --pos; return *this; }
        iterator operator--(int) { iterator tmp = *this; --pos; return tmp; }

        iterator& operator+=(ptrdiff_t n) { pos += n; return *this; }
        iterator& operator-=(ptrdiff_t n) { pos -= n; return *this; }
        iterator operator+(ptrdiff_t n) const { return iterator(deque, pos + n); }
        iterator operator-(ptrdiff_t n) const { return iterator(deque, pos - n); }
        friend iterator operator+(ptrdiff_t n, const iterator& it) { return it + n; }
        ptrdiff_t operator-(const iterator& other) const { return pos - other.pos; }

        T& operator[](ptrdiff_t n) const { return (*deque)[pos + n]; }
        bool operator==(const iterator& other) const { return deque == other.deque && pos == other.pos; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        bool operator<(const iterator& other) const { return pos < other.pos; }
        bool operator>(const iterator& other) const { return pos > other.pos; }
        bool operator<=(const iterator& other) const { return pos <= other.pos; }
        bool operator>=(const iterator& other) const { return pos >= other.pos; }
    };

    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size_); }
};

#endif 