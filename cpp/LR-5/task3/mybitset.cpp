#include "mybitset.h"
#include <stdexcept>
#include <climits>

static constexpr size_t BITS_PER_WORD = 32u;

static size_t wordsNeeded(size_t n)
{
    return n == 0 ? 0 : (n + BITS_PER_WORD - 1) / BITS_PER_WORD;
}

// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------
MyBitSet::MyBitSet(size_t n)
    : m_size(n)
    , m_words(wordsNeeded(n), 0u)
{}

MyBitSet::MyBitSet(size_t n, unsigned long long val)
    : m_size(n)
    , m_words(wordsNeeded(n), 0u)
{
    // Fill words from val
    for (size_t w = 0; w < m_words.size() && val != 0; ++w) {
        m_words[w] = static_cast<uint32_t>(val & 0xFFFFFFFFu);
        val >>= 32;
    }
    maskLastWord();
}

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------
void MyBitSet::maskLastWord()
{
    if (m_size == 0) return;
    size_t rem = m_size % BITS_PER_WORD;
    if (rem != 0)
        m_words.back() &= (1u << rem) - 1u;
}

void MyBitSet::set_bit(size_t pos, bool val)
{
    size_t   word = pos / BITS_PER_WORD;
    uint32_t bit  = 1u << (pos % BITS_PER_WORD);
    if (val) m_words[word] |= bit;
    else     m_words[word] &= ~bit;
}

// ---------------------------------------------------------------------------
// Element access — O(1)
// ---------------------------------------------------------------------------
bool MyBitSet::test(size_t pos) const
{
    if (pos >= m_size)
        throw std::out_of_range("MyBitSet::test — index out of range");
    return (m_words[pos / BITS_PER_WORD] >> (pos % BITS_PER_WORD)) & 1u;
}

// ---------------------------------------------------------------------------
// Bulk queries — O(N/32)
// ---------------------------------------------------------------------------
bool MyBitSet::all() const
{
    if (m_size == 0) return true;
    // All full words must be 0xFFFFFFFF
    for (size_t w = 0; w + 1 < m_words.size(); ++w)
        if (m_words[w] != 0xFFFFFFFFu) return false;
    // Last word: only the used bits must be 1
    size_t rem = m_size % BITS_PER_WORD;
    uint32_t mask = (rem == 0) ? 0xFFFFFFFFu : ((1u << rem) - 1u);
    return (m_words.back() & mask) == mask;
}

bool MyBitSet::any() const
{
    for (uint32_t w : m_words)
        if (w != 0u) return true;
    return false;
}

bool MyBitSet::none() const
{
    return !any();
}

size_t MyBitSet::count() const
{
    size_t cnt = 0;
    for (uint32_t w : m_words) {
        // Kernighan bit-count
        uint32_t v = w;
        while (v) { v &= v - 1; ++cnt; }
    }
    return cnt;
}

// ---------------------------------------------------------------------------
// Single-bit modifiers — O(1)
// ---------------------------------------------------------------------------
MyBitSet& MyBitSet::set(size_t pos, bool val)
{
    if (pos >= m_size) throw std::out_of_range("MyBitSet::set");
    set_bit(pos, val);
    return *this;
}

MyBitSet& MyBitSet::reset(size_t pos)
{
    if (pos >= m_size) throw std::out_of_range("MyBitSet::reset");
    set_bit(pos, false);
    return *this;
}

MyBitSet& MyBitSet::flip(size_t pos)
{
    if (pos >= m_size) throw std::out_of_range("MyBitSet::flip");
    m_words[pos / BITS_PER_WORD] ^= (1u << (pos % BITS_PER_WORD));
    return *this;
}

// ---------------------------------------------------------------------------
// Bulk modifiers — O(N/32)
// ---------------------------------------------------------------------------
MyBitSet& MyBitSet::set()
{
    for (auto& w : m_words) w = 0xFFFFFFFFu;
    maskLastWord();
    return *this;
}

MyBitSet& MyBitSet::reset()
{
    for (auto& w : m_words) w = 0u;
    return *this;
}

MyBitSet& MyBitSet::flip()
{
    for (auto& w : m_words) w = ~w;
    maskLastWord();
    return *this;
}

// ---------------------------------------------------------------------------
// Bitwise operators — O(N/32)
// ---------------------------------------------------------------------------
MyBitSet MyBitSet::operator~() const
{
    MyBitSet result(*this);
    result.flip();
    return result;
}

MyBitSet MyBitSet::operator&(const MyBitSet& rhs) const
{
    MyBitSet result(m_size);
    size_t n = std::min(m_words.size(), rhs.m_words.size());
    for (size_t i = 0; i < n; ++i)
        result.m_words[i] = m_words[i] & rhs.m_words[i];
    return result;
}

MyBitSet MyBitSet::operator|(const MyBitSet& rhs) const
{
    MyBitSet result(m_size);
    size_t n = std::min(m_words.size(), rhs.m_words.size());
    for (size_t i = 0; i < n; ++i)
        result.m_words[i] = m_words[i] | rhs.m_words[i];
    result.maskLastWord();
    return result;
}

MyBitSet& MyBitSet::operator&=(const MyBitSet& rhs)
{
    size_t n = std::min(m_words.size(), rhs.m_words.size());
    for (size_t i = 0; i < n; ++i) m_words[i] &= rhs.m_words[i];
    return *this;
}

MyBitSet& MyBitSet::operator|=(const MyBitSet& rhs)
{
    size_t n = std::min(m_words.size(), rhs.m_words.size());
    for (size_t i = 0; i < n; ++i) m_words[i] |= rhs.m_words[i];
    maskLastWord();
    return *this;
}

// ---------------------------------------------------------------------------
// Conversions — O(N/32)
// ---------------------------------------------------------------------------
std::string MyBitSet::to_string() const
{
    std::string s(m_size, '0');
    for (size_t i = 0; i < m_size; ++i)
        if (test(i)) s[m_size - 1 - i] = '1'; // MSB on the left
    return s;
}

unsigned long MyBitSet::to_ulong() const
{
    unsigned long result = 0;
    size_t bits = std::min(m_size, size_t(sizeof(unsigned long) * 8));
    for (size_t i = 0; i < bits; ++i)
        if (test(i)) result |= (1ul << i);
    return result;
}

unsigned long long MyBitSet::to_ullong() const
{
    unsigned long long result = 0;
    size_t bits = std::min(m_size, size_t(sizeof(unsigned long long) * 8));
    for (size_t i = 0; i < bits; ++i)
        if (test(i)) result |= (1ull << i);
    return result;
}
