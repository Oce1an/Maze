#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <forward_list>
#include <vector>
#include <utility>
#include <functional>
#include <stdexcept>

template<typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
class UnorderedMap {
public:
    using value_type = std::pair<const KeyType, ValueType>;
    using bucket_type = std::forward_list<value_type>;

    UnorderedMap(size_t bucketCount = 8, const Hash& hash = Hash())
        : buckets(bucketCount), hashFunc(hash), sz(0) {}

    std::pair<typename bucket_type::iterator, bool> insert(const KeyType& key, const ValueType& value);

    ValueType& operator[](const KeyType& key);
    ValueType& at(const KeyType& key);
    const ValueType& at(const KeyType& key) const;

    bool contains(const KeyType& key) const;
    bool erase(const KeyType& key);
    void clear();
    void rehash(size_t newBucketCount);

    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    size_t bucket_count() const { return buckets.size(); }

    std::vector<std::pair<KeyType, ValueType>> items() const;

private:
    std::vector<bucket_type> buckets;
    Hash hashFunc;
    size_t sz;

    typename bucket_type::iterator find_in_bucket(bucket_type& bucket, const KeyType& key);
    typename bucket_type::const_iterator find_in_bucket(const bucket_type& bucket, const KeyType& key) const;
};

template<typename K, typename V, typename H>
std::pair<typename std::forward_list<std::pair<const K,V>>::iterator, bool>
UnorderedMap<K,V,H>::insert(const K& key, const V& value) {
    size_t idx = hashFunc(key) % buckets.size();
    auto& bucket = buckets[idx];
    auto it = find_in_bucket(bucket, key);
    if (it != bucket.end()) return {it, false};
    bucket.emplace_front(key, value);
    ++sz;
    if (sz > buckets.size() * 0.75) rehash(buckets.size() * 2);
    return {bucket.begin(), true};
}

template<typename K, typename V, typename H>
V& UnorderedMap<K,V,H>::operator[](const K& key) {
    size_t idx = hashFunc(key) % buckets.size();
    auto& bucket = buckets[idx];
    auto it = find_in_bucket(bucket, key);
    if (it == bucket.end()) {
        bucket.emplace_front(key, V{});
        ++sz;
        if (sz > buckets.size() * 0.75) rehash(buckets.size() * 2);
        return bucket.begin()->second;
    }
    return it->second;
}

template<typename K, typename V, typename H>
V& UnorderedMap<K,V,H>::at(const K& key) {
    size_t idx = hashFunc(key) % buckets.size();
    auto& bucket = buckets[idx];
    auto it = find_in_bucket(bucket, key);
    if (it == bucket.end()) throw std::out_of_range("UnorderedMap::at");
    return it->second;
}

template<typename K, typename V, typename H>
const V& UnorderedMap<K,V,H>::at(const K& key) const {
    size_t idx = hashFunc(key) % buckets.size();
    const auto& bucket = buckets[idx];
    auto it = find_in_bucket(bucket, key);
    if (it == bucket.end()) throw std::out_of_range("UnorderedMap::at");
    return it->second;
}

template<typename K, typename V, typename H>
bool UnorderedMap<K,V,H>::contains(const K& key) const {
    size_t idx = hashFunc(key) % buckets.size();
    const auto& bucket = buckets[idx];
    return find_in_bucket(bucket, key) != bucket.end();
}

template<typename K, typename V, typename H>
bool UnorderedMap<K,V,H>::erase(const K& key) {
    size_t idx = hashFunc(key) % buckets.size();
    auto& bucket = buckets[idx];
    auto prev = bucket.before_begin();
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
            bucket.erase_after(prev);
            --sz;
            return true;
        }
        prev = it;
    }
    return false;
}

template<typename K, typename V, typename H>
void UnorderedMap<K,V,H>::clear() {
    for (auto& b : buckets) b.clear();
    sz = 0;
}

template<typename K, typename V, typename H>
void UnorderedMap<K,V,H>::rehash(size_t newCount) {
    if (newCount == 0) newCount = 1;
    std::vector<bucket_type> newBuckets(newCount);
    for (auto& bucket : buckets) {
        for (auto it = bucket.begin(); it != bucket.end(); ) {
            auto next = it; ++next;
            size_t newIdx = hashFunc(it->first) % newCount;
            newBuckets[newIdx].splice_after(newBuckets[newIdx].before_begin(), bucket, it);
            it = next;
        }
    }
    buckets.swap(newBuckets);
}

template<typename K, typename V, typename H>
typename UnorderedMap<K,V,H>::bucket_type::iterator
UnorderedMap<K,V,H>::find_in_bucket(bucket_type& bucket, const K& key) {
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
        if (it->first == key) return it;
    return bucket.end();
}

template<typename K, typename V, typename H>
typename UnorderedMap<K,V,H>::bucket_type::const_iterator
UnorderedMap<K,V,H>::find_in_bucket(const bucket_type& bucket, const K& key) const {
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
        if (it->first == key) return it;
    return bucket.end();
}

template<typename K, typename V, typename H>
std::vector<std::pair<K,V>> UnorderedMap<K,V,H>::items() const {
    std::vector<std::pair<K,V>> result;
    for (const auto& bucket : buckets) {
        for (const auto& p : bucket) result.emplace_back(p.first, p.second);
    }
    return result;
}
#endif