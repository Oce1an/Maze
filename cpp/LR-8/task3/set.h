#ifndef SET_H
#define SET_H

#include "map.h"

template<typename KeyType>
class Set : public Map<KeyType, char> {
public:
    bool insert(const KeyType& key) {
        return Map<KeyType, char>::insert(key, '\0');
    }
    void operator[](const KeyType&) = delete;
};
#endif