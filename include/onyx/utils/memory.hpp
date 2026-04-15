// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 内存访问工具模板。
//

#pragma once
#ifndef ONYX_UTILS_MEMORY_HPP
#define ONYX_UTILS_MEMORY_HPP

#include <KittyMemory.hpp>
#include <cstdint>
#include <optional>

namespace onyx::utils {

class Memory final {
public:
    /// @brief 从内存地址安全地读取类型为 T 的值。
    /// @tparam T 要读取的数据类型。
    /// @param address 绝对内存地址。
    /// @returns 读取的值，如果读取失败则返回 T 的默认值。
    template<typename T>
    [[nodiscard]] static T read(uintptr_t address) noexcept {
        T buffer{};
        if (KittyMemory::syscallMemRead(address, &buffer, sizeof(T)) <= 0) {
            return T{};
        }
        return buffer;
    }

    /// @brief 安全地读取类型为 T 的值并将其作为 std::optional 返回。
    template<typename T>
    [[nodiscard]] static std::optional<T> try_read(uintptr_t address) noexcept {
        T buffer{};
        if (KittyMemory::syscallMemRead(address, &buffer, sizeof(T)) > 0) {
            return buffer;
        }
        return std::nullopt;
    }
};

} // namespace onyx::utils

#endif // ONYX_UTILS_MEMORY_HPP
