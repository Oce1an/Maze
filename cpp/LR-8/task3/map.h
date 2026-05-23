#ifndef MAP_H
#define MAP_H

#include "bst.h"

template<typename KeyType, typename ValueType>
class Map : public BST<KeyType, ValueType> {
public:
    ValueType& operator[](const KeyType& key);
    ValueType& at(const KeyType& key);
    const ValueType& at(const KeyType& key) const;
    using BST<KeyType, ValueType>::insert;
};

template<typename K, typename V>
V& Map<K,V>::operator[](const K& key) {
    auto node = this->find(key);
    if (!node) {
        this->insert(key, V{});
        node = this->find(key);
    }
    return node->data.second;
}

template<typename K, typename V>
V& Map<K,V>::at(const K& key) {
    auto node = this->find(key);
    if (!node) throw std::out_of_range("Map::at: key not found");
    return node->data.second;
}

template<typename K, typename V>
const V& Map<K,V>::at(const K& key) const {
    auto node = this->find(key);
    if (!node) throw std::out_of_range("Map::at: key not found");
    return node->data.second;
}
#endif