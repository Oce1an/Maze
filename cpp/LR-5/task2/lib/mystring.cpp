#include "mystring.h"

#include "mystring.h"

void MyString::allocate(size_t capacity)
{
    m_cap = capacity + 1;
    m_data.reset(new char[m_cap]);
    m_data[0] = '\0';
}

MyString::MyString()
    : m_len(0), m_cap(0)
{
    allocate(0);
}

MyString::MyString(const char* s)
    : m_len(0), m_cap(0)
{
    if (!s) s = "";
    m_len = s_strlen(s);
    allocate(m_len);
    s_memcpy(m_data.get(), s, m_len + 1);
}

MyString::MyString(const MyString& other)
    : m_len(other.m_len), m_cap(0)
{
    allocate(m_len);
    s_memcpy(m_data.get(), other.m_data.get(), m_len + 1);
}

MyString& MyString::operator=(const MyString& other)
{
    if (this == &other) return *this;
    m_len = other.m_len;
    allocate(m_len);
    s_memcpy(m_data.get(), other.m_data.get(), m_len + 1);
    return *this;
}

char& MyString::operator[](size_t pos)
{
    if (pos >= m_len) {
        static char dummy = '\0';
        return dummy;
    }
    return m_data[pos];
}

const char& MyString::operator[](size_t pos) const
{
    if (pos >= m_len) {
        static char dummy = '\0';
        return dummy;
    }
    return m_data[pos];
}

void* MyString::s_memcpy(void* s1, const void* s2, size_t n)
{
    unsigned char*       dst = static_cast<unsigned char*>(s1);
    const unsigned char* src = static_cast<const unsigned char*>(s2);
    for (size_t i = 0; i < n; ++i)
        dst[i] = src[i];
    return s1;
}

void* MyString::s_memmove(void* s1, const void* s2, size_t n)
{
    unsigned char*       dst = static_cast<unsigned char*>(s1);
    const unsigned char* src = static_cast<const unsigned char*>(s2);
    if (dst < src) {
        for (size_t i = 0; i < n; ++i) dst[i] = src[i];
    } else if (dst > src) {
        for (size_t i = n; i > 0; --i) dst[i-1] = src[i-1];
    }
    return s1;
}

char* MyString::s_strcpy(char* s1, const char* s2)
{
    char* ret = s1;
    while ((*s1++ = *s2++) != '\0') {}
    return ret;
}

char* MyString::s_strncpy(char* s1, const char* s2, size_t n)
{
    char* ret = s1;
    size_t i = 0;
    while (i < n && s2[i] != '\0') { s1[i] = s2[i]; ++i; }
    while (i < n) { s1[i++] = '\0'; }
    return ret;
}

char* MyString::s_strcat(char* s1, const char* s2)
{
    char* ret = s1;
    while (*s1) ++s1;
    while ((*s1++ = *s2++) != '\0') {}
    return ret;
}

char* MyString::s_strncat(char* s1, const char* s2, size_t n)
{
    char* ret = s1;
    while (*s1) ++s1;
    size_t i = 0;
    while (i < n && s2[i] != '\0') { s1[i] = s2[i]; ++i; }
    s1[i] = '\0';
    return ret;
}

int MyString::s_memcmp(const void* s1, const void* s2, size_t n)
{
    const unsigned char* a = static_cast<const unsigned char*>(s1);
    const unsigned char* b = static_cast<const unsigned char*>(s2);
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return static_cast<int>(a[i]) - static_cast<int>(b[i]);
    }
    return 0;
}

int MyString::s_strcmp(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) { ++s1; ++s2; }
    return static_cast<int>(static_cast<unsigned char>(*s1))
         - static_cast<int>(static_cast<unsigned char>(*s2));
}

int MyString::s_strcoll(const char* s1, const char* s2)
{
    return s_strcmp(s1, s2);
}

int MyString::s_strncmp(const char* s1, const char* s2, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        unsigned char a = static_cast<unsigned char>(s1[i]);
        unsigned char b = static_cast<unsigned char>(s2[i]);
        if (a != b) return static_cast<int>(a) - static_cast<int>(b);
        if (a == '\0') return 0;
    }
    return 0;
}

size_t MyString::s_strxfrm(char* s1, const char* s2, size_t n)
{
    size_t len = s_strlen(s2);
    if (n > 0) {
        size_t copy_len = (len < n) ? len : n - 1;
        s_memcpy(s1, s2, copy_len);
        s1[copy_len] = '\0';
    }
    return len;
}

char* MyString::s_strtok(char* s1, const char* s2)
{
    static char* saved = 0;
    if (s1) saved = s1;
    if (!saved) return 0;

    while (*saved) {
        const char* d = s2;
        while (*d) {
            if (*saved == *d) break;
            ++d;
        }
        if (*d == '\0') break;
        ++saved;
    }

    if (*saved == '\0') {
        saved = 0;
        return 0;
    }

    char* token = saved;

    while (*saved) {
        const char* d = s2;
        while (*d) {
            if (*saved == *d) {
                *saved = '\0';
                ++saved;
                return token;
            }
            ++d;
        }
        ++saved;
    }

    saved = 0;
    return token;
}

void* MyString::s_memset(void* s, int c, size_t n)
{
    unsigned char* p = static_cast<unsigned char*>(s);
    for (size_t i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(c);
    return s;
}

char* MyString::s_strerror(int errnum)
{
    static char buffer[32];
    if (errnum == 0) {
        s_strcpy(buffer, "No error");
    } else {
        s_strcpy(buffer, "Unknown error");
        char* p = buffer;
        while (*p) ++p;
        *p++ = ' ';
        *p++ = '#';
        if (errnum < 0) {
            *p++ = '-';
            errnum = -errnum;
        }
        char temp[16];
        int i = 0;
        do {
            temp[i++] = '0' + (errnum % 10);
            errnum /= 10;
        } while (errnum > 0 && i < 15);
        while (i > 0) *p++ = temp[--i];
        *p = '\0';
    }
    return buffer;
}

size_t MyString::s_strlen(const char* s)
{
    size_t n = 0;
    while (s[n]) ++n;
    return n;
}