// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 用于管理玩家集合的 GameSystem 的 SDK 包装器。
//

#pragma once
#ifndef ONYX_SDK_GAME_SYSTEM_HPP
#define ONYX_SDK_GAME_SYSTEM_HPP

#include <onyx/sdk/offsets.hpp>
#include <onyx/il2cpp/collection.hpp>
#include <onyx/utils/memory.hpp>

namespace onyx::sdk {

class GameSystem final {
public:
    explicit GameSystem(void* address) noexcept : m_address(reinterpret_cast<uintptr_t>(address)) {}

    [[nodiscard]] bool is_valid() const noexcept { return m_address != 0; }

    [[nodiscard]] il2cpp::Dictionary<int32_t, void*>* player_dict() const noexcept {
        return utils::Memory::read<il2cpp::Dictionary<int32_t, void*>*>(m_address + offsets::game_system::k_all_players_dict);
    }

private:
    uintptr_t m_address;
};

} // namespace onyx::sdk

#endif // ONYX_SDK_GAME_SYSTEM_HPP
