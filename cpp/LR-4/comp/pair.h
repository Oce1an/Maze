#ifndef PAIR_H
#define PAIR_H

template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair() : first(), second() {}

    Pair(const T1& f, const T2& s) : first(f), second(s) {}

    Pair(const Pair& other) : first(other.first), second(other.second) {}

    Pair& operator=(const Pair& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    friend bool operator==(const Pair& a, const Pair& b) {
        return a.first == b.first && a.second == b.second;
    }
};

#endif