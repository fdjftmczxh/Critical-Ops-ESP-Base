// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// ESP 逻辑和玩家数据管理。
//

#pragma once
#ifndef ONYX_CORE_ESP_MANAGER_HPP
#define ONYX_CORE_ESP_MANAGER_HPP

#include <onyx/il2cpp/api.hpp>
#include <onyx/core/camera_system.hpp>
#include <vector>
#include <mutex>
#include <imgui.h>

namespace onyx::core {

struct BoneLine {
    ImVec2 p1;
    ImVec2 p2;
};

struct PlayerData {
    ImVec2 screen_pos;
    bool   is_visible{false};
    std::vector<BoneLine> skeleton_lines;
};

struct BoneConnection {
    int32_t from;
    int32_t to;
};

class EspManager final {
public:
    EspManager() = default;
    ~EspManager() = default;

    // 不可复制
    EspManager(const EspManager&) = delete;
    EspManager& operator=(const EspManager&) = delete;

    /// @brief 定期从游戏世界中获取所有玩家数据。
    /// 应在后台线程运行。
    void update_loop(uintptr_t base_address, const CameraSystem& camera) noexcept;

    /// @brief 安全地检索当前玩家数据的副本。
    [[nodiscard]] std::vector<PlayerData> players() const noexcept;

private:
    /// @brief 清除当前玩家列表。
    void clear_players() noexcept;

    mutable std::mutex           m_mutex;
    std::vector<PlayerData>      m_players;
    
    static const BoneConnection k_skeleton_connections[];
};

} // namespace onyx::core

#endif // ONYX_CORE_ESP_MANAGER_HPP
