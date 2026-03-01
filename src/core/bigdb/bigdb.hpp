#pragma once

#include <ctime>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>
#include <coroutine>
using namespace std;


class bigdbsharded{
    const size_t m_mask;
    vector<bigdblock> m_shards;
    bigdblock& get_shard(const string &key){
        hash<string> hash_fn;
        auto h = hash_fn(key);
        return m_shards[h & m_mask];
    }
    public:
        bigdbsharded(size_t num_shard): m_mask(num_shard-1), m_shards(num_shard) {
            //this->m_mask = num_shard - 1;
            //this->m_shards = vector<bigdbblock>(num_shard);
        }
        void put(const string &key, int value) {
            get_shard(key).put(key, value);
        }

        optional<int> get(const string &key) {
            return get_shard(key).get(key);
        }

        bool remove(const string &key) {
            return get_shard(key).remove(key);
        }
};

class bigdblock {
    mutex m_mutex;
    unordered_map<string, int> m_map;

public:
    void put(const string &key, int value) {
        lock_guard lock(m_mutex);
        m_map.emplace(key, value);
    }

    optional<int> get(const string &key) {
        lock_guard lock(m_mutex);
        auto it = m_map.find(key);
        if (it != m_map.end())
            return it->second;
        return {};
    }

    bool remove(const string &key) {
        lock_guard lock(m_mutex);
        auto n = m_map.erase(key);
        return n;
    }
};