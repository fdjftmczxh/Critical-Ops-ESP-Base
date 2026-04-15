// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 集中管理游戏偏移量和混淆名称映射。
//

#pragma once
#ifndef ONYX_SDK_OFFSETS_HPP
#define ONYX_SDK_OFFSETS_HPP

#include <cstdint>

namespace onyx::sdk::offsets {

// ─── RVA (相对虚拟地址) ──────────────────────────────────────────────────────

/// @brief OECFB7135E.OA96CB9447() : GameplayModule.get_Instance()
constexpr uintptr_t k_rva_gameplay_module_get_instance = 0x190326a;

/// @brief UnityEngine.SkinnedMeshRenderer.get_bones()
constexpr uintptr_t k_rva_get_bones = 0x296d2a0;

// ─── 类偏移量 ──────────────────────────────────────────────────────────────

/// @brief 类: OECFB7135E (GameplayModule)
namespace gameplay_module {
    /// @brief 字段: OE92D655B7 (GameSystem)
    constexpr int32_t k_game_system = 0x30;
}

/// @brief 类: O6F692BC0C (GameSystem)
namespace game_system {
    /// @brief 字段: OE54334CA1 (Dictionary<int, Character>)
    constexpr int32_t k_all_players_dict = 0x30;
}

namespace character {
    /// @brief 字段: OD3B38604D (CharacterModel)
    constexpr int32_t k_model = 0x168;
}

/// @brief 类: O5EF6CBE7B (CharacterModel)
namespace character_model {
    /// @brief 字段: O82DE28BAB (SkinnedMeshRenderer)
    constexpr int32_t k_renderer = 0x20;
}

} // namespace onyx::sdk::offsets

#endif // ONYX_SDK_OFFSETS_HPP
