// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 由 Unity 的 SkinnedMeshRenderer 处理的 CharacterModel 的 SDK 包装器。
//

#pragma once
#ifndef ONYX_SDK_CHARACTER_MODEL_HPP
#define ONYX_SDK_CHARACTER_MODEL_HPP

#include <onyx/sdk/offsets.hpp>
#include <onyx/il2cpp/collection.hpp>
#include <onyx/utils/memory.hpp>

namespace onyx::sdk {

class CharacterModel final {
public:
    explicit CharacterModel(void* address) noexcept : m_address(reinterpret_cast<uintptr_t>(address)) {}

    [[nodiscard]] bool is_valid() const noexcept { return m_address != 0; }

    /// @brief 从 SkinnedMeshRenderer 中检索原生骨骼数组。
    /// @param base_address 用于 RVA 解析的游戏库基地址。
    [[nodiscard]] il2cpp::ManagedArray<uintptr_t>* get_bones(uintptr_t base_address) const noexcept {
        auto renderer = utils::Memory::read<void*>(m_address + offsets::character_model::k_renderer);
        if (!renderer) return nullptr;

        using t_get_bones = void* (*)(void*);
        auto get_bones_func = reinterpret_cast<t_get_bones>(base_address + offsets::k_rva_get_bones);
        
        return reinterpret_cast<il2cpp::ManagedArray<uintptr_t>*>(get_bones_func(renderer));
    }

private:
    uintptr_t m_address;
};

} // namespace onyx::sdk

#endif // ONYX_SDK_CHARACTER_MODEL_HPP
