#ifndef MYBITSET_H
#define MYBITSET_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>

// ---------------------------------------------------------------------------
// MyBitSet — dynamic bitset with O(N/32) bulk operations
// Internal storage: array of uint32_t words
// ---------------------------------------------------------------------------
class MyBitSet
{
public:
    // ---- Proxy for operator[] (non-const) ----
    class reference {
    public:
        reference(MyBitSet& bs, size_t pos) : m_bs(bs), m_pos(pos) {}
        reference& operator=(bool val) { m_bs.set_bit(m_pos, val); return *this; }
        operator bool() const { return m_bs.test(m_pos); }
    private:
        MyBitSet& m_bs;
        size_t    m_pos;
    };

    // ---- Constructors ----
    explicit MyBitSet(size_t n = 0);                 // all zeros
    MyBitSet(size_t n, unsigned long long val);      // initialise from integer

    // ---- Element access — O(1) ----
    reference operator[](size_t pos)       { return reference(*this, pos); }
    bool      operator[](size_t pos) const { return test(pos); }

    bool test(size_t pos) const;   // throws std::out_of_range if pos >= size

    // ---- Queries — O(1) ----
    size_t size()  const { return m_size; }
    bool   empty() const { return m_size == 0; }

    // ---- Bulk queries — O(N/32) ----
    bool   all()  const;
    bool   any()  const;
    bool   none() const;
    size_t count() const;

    // ---- Modifiers — O(1) single bit ----
    MyBitSet& set(size_t pos, bool val = true);   // set one bit
    MyBitSet& reset(size_t pos);                  // clear one bit
    MyBitSet& flip(size_t pos);                   // toggle one bit

    // ---- Bulk modifiers — O(N/32) ----
    MyBitSet& set();    // set all bits to 1
    MyBitSet& reset();  // set all bits to 0
    MyBitSet& flip();   // toggle all bits

    // ---- Bitwise operators — O(N/32) ----
    MyBitSet  operator~()              const;
    MyBitSet  operator&(const MyBitSet& rhs) const;
    MyBitSet  operator|(const MyBitSet& rhs) const;
    MyBitSet& operator&=(const MyBitSet& rhs);
    MyBitSet& operator|=(const MyBitSet& rhs);

    // ---- Conversions — O(N/32) ----
    std::string        to_string()  const;
    unsigned long      to_ulong()   const;
    unsigned long long to_ullong()  const;

private:
    size_t                m_size;   // number of logical bits
    std::vector<uint32_t> m_words;  // storage: ceil(m_size/32) words

    size_t wordCount() const { return m_words.size(); }
    void   maskLastWord();           // zero out unused high bits in last word

    // Internal helper — no bounds check
    void   set_bit(size_t pos, bool val);
};

#endif // MYBITSET_H
