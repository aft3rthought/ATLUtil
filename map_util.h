

#pragma once

#include <map>

namespace atl
{
    template <typename K, typename V>
    bool map_has(const std::map<K,V> & map, const K & key)
    {
        return map.find(key) != map.end();
    }
}