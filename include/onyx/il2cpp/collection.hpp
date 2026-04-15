// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 使用官方引擎头文件的 IL2CPP 集合类型的现代 C++ 包装器。
//

#pragma once
#ifndef ONYX_IL2CPP_COLLECTION_HPP
#define ONYX_IL2CPP_COLLECTION_HPP

#include <onyx/il2cpp/api.hpp>
#include <optional>

namespace onyx::il2cpp {

/// @brief IL2CPP 原生 Array 结构体的类型安全包装器。
/// @tparam T 元素类型。
template <typename T>
struct ManagedArray : public Array {
    T vector[1]; // 可变长度数组成员 (请谨慎使用)

    [[nodiscard]] T& operator[](int32_t i) noexcept { return vector[i]; }
    [[nodiscard]] const T& operator[](int32_t i) const noexcept { return vector[i]; }

    [[nodiscard]] int32_t length() const noexcept { return max_length; }

    [[nodiscard]] bool contains(const T& item) const noexcept {
        for (int32_t i = 0; i < max_length; ++i) {
            if (vector[i] == item) return true;
        }
        return false;
    }
};

/// @brief 镜像原生 IL2CPP Dictionary。
template<typename TKey, typename TValue> 
struct Dictionary {
    struct Entry {
        int32_t hashCode; 
        int32_t next;
        TKey    key;
        TValue  value;
    };

    void* klass;
    void* monitor;
    ManagedArray<int32_t>* buckets;
    ManagedArray<Entry>*   entries;
    int32_t count;
    int32_t version;
    int32_t freeList;
    int32_t freeCount;
    void*   comparer;
    void*   keys;
    void*   values;
    void*   syncRoot;

    [[nodiscard]] int32_t size() const noexcept { return count; }

    [[nodiscard]] std::optional<TValue> try_get_value(const TKey& key) const noexcept {
        int index = find_entry(key);
        if (index >= 0) return entries->vector[index].value;
        return std::nullopt;
    }

    [[nodiscard]] int32_t find_entry(const TKey& key) const noexcept {
        if (!entries) return -1;
        for (int i = 0; i < count; ++i) {
            if (entries->vector[i].key == key) return i;
        }
        return -1;
    }

    [[nodiscard]] bool contains_key(const TKey& key) const noexcept {
        return find_entry(key) >= 0;
    }
};

} // namespace onyx::il2cpp

#endif // ONYX_IL2CPP_COLLECTION_HPP
