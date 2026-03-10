#pragma once

#include <atomic>
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>
#include <optional>    
#include <functional> 

using namespace std;

template <typename K, typename V>
class bigdblock {
    mutex m_mutex;
    unordered_map<K, V> m_map; 

public:
    void put(const K &key, const V &value) {
        lock_guard<mutex> lock(m_mutex);
        // Note: m_map[key] = value overwrites existing keys. 
        m_map[key] = value; 
    }

    optional<V> get(const K &key) {
        lock_guard<mutex> lock(m_mutex);
        auto it = m_map.find(key);
        if (it != m_map.end())
            return it->second;
        return {};
    }

    bool remove(const K &key) {
        lock_guard<mutex> lock(m_mutex);
        return m_map.erase(key) > 0;
    }
};

template <typename K, typename V>
class bigdbsharded {
    const size_t m_mask;
    // You MUST specify the types when using templates. Holy C++ syntax errors. 
    vector<bigdblock<K, V>> m_shards; 

    bigdblock<K, V>& get_shard(const K &key) {
        hash<K> hash_fn; 
        auto h = hash_fn(key);
        return m_shards[h & m_mask];
    }

public:
    // Note: num_shard MUST be a power of 2. Don't be stupid. 
    bigdbsharded(size_t num_shard): m_mask(num_shard - 1), m_shards(num_shard) {}

    void put(const K &key, const V &value) {
        get_shard(key).put(key, value);
    }

    optional<V> get(const K &key) {
        return get_shard(key).get(key);
    }

    bool remove(const K &key) {
        return get_shard(key).remove(key);
    }
};

template <typename K, typename V>
class bigdb {
    const size_t num_map;
    vector<bigdbsharded<K, V>> replicated_map;

public:
    // Initialize the vector with 'num_maps' replicas. 
    // We arbitrarily give each shard 16 locks here as an example.
    bigdb(size_t num_maps) : num_map(num_maps), replicated_map(num_maps, bigdbsharded<K, V>(16)) {}
    
    void put(const K &key, const V &value){
        for(auto& shard : replicated_map) {
            shard.put(key, value);
        }
    }

    void get(const K & key){

    }
};