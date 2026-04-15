// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 基础 Character 类的 SDK 包装器。
//

#pragma once
#ifndef ONYX_SDK_CHARACTER_HPP
#define ONYX_SDK_CHARACTER_HPP

#include <onyx/sdk/offsets.hpp>
#include <onyx/sdk/character_model.hpp>
#include <onyx/il2cpp/api.hpp>
#include <onyx/utils/memory.hpp>
#include <android/log.h>

namespace onyx::sdk {

class Character final {
public:
    explicit Character(void* address) noexcept : m_address(reinterpret_cast<uintptr_t>(address)) {}

    [[nodiscard]] bool is_valid() const noexcept { return m_address != 0; }

    [[nodiscard]] CharacterModel model() const noexcept {
        auto addr = utils::Memory::read<void*>(m_address + offsets::character::k_model);
        return CharacterModel(addr);
    }

    [[nodiscard]] il2cpp::Transform* transform() const noexcept {
        return il2cpp::unity::get_transform(reinterpret_cast<il2cpp::Component*>(m_address));
    }

    [[nodiscard]] il2cpp::Vector3 position() const noexcept {
        auto* trans = transform();
        if (!trans) return {0, 0, 0};
        return il2cpp::unity::get_position(trans);
    }

private:
    uintptr_t m_address;
};

} // namespace onyx::sdk

#endif // ONYX_SDK_CHARACTER_HPP
