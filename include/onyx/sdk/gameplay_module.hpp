// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 主 GameplayModule 的 SDK 包装器。
//

#pragma once
#ifndef ONYX_SDK_GAMEPLAY_MODULE_HPP
#define ONYX_SDK_GAMEPLAY_MODULE_HPP

#include <onyx/sdk/offsets.hpp>
#include <onyx/sdk/game_system.hpp>
#include <onyx/utils/memory.hpp>

namespace onyx::sdk {

class GameplayModule final {
public:
    explicit GameplayModule(void* address) noexcept : m_address(reinterpret_cast<uintptr_t>(address)) {}
    
    [[nodiscard]] static GameplayModule get_instance(uintptr_t base_address) noexcept {
        using t_get_singleton = void* (*)();
        auto get_singleton = reinterpret_cast<t_get_singleton>(base_address + offsets::k_rva_gameplay_module_get_instance);
        return GameplayModule(get_singleton());
    }

    [[nodiscard]] bool is_valid() const noexcept { return m_address != 0; }

    [[nodiscard]] GameSystem game_system() const noexcept {
        auto addr = utils::Memory::read<void*>(m_address + offsets::gameplay_module::k_game_system);
        return GameSystem(addr);
    }

private:
    uintptr_t m_address;
};

} // namespace onyx::sdk

#endif // ONYX_SDK_GAMEPLAY_MODULE_HPP
