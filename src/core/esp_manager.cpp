// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 使用 Onyx 游戏 SDK 实现 ESP 系统。
//

#include <onyx/core/esp_manager.hpp>
#include <onyx/sdk/gameplay_module.hpp>
#include <onyx/sdk/character.hpp>
#include <onyx/utils/memory.hpp>
#include <unistd.h>
#include <algorithm>

namespace onyx::core {

using namespace sdk;
using namespace il2cpp;
using namespace utils;

const BoneConnection EspManager::k_skeleton_connections[] = {
    {0, 1}, {1, 3}, {3, 4}, {4, 5},           // 脊柱和头部
    {3, 31}, {31, 33}, {33, 34}, {34, 35},    // 左臂
    {3, 6}, {6, 8}, {8, 12}, {12, 13},        // 右臂
    {0, 58}, {58, 61}, {61, 59}, {59, 60},    // 左腿
    {0, 64}, {64, 67}, {67, 65}, {65, 66}     // 右腿
};

void EspManager::update_loop(uintptr_t base_address, const CameraSystem& camera) noexcept {
    auto domain = api::domain_get();
    api::thread_attach(domain);

    while (true) {
        usleep(10000); // 100hz 更新

        // ─── 通过 SDK 解析场景内容 ──────────────────────────────────────────
        auto gameplay = GameplayModule::get_instance(base_address);
        if (!gameplay.is_valid()) {
            clear_players();
            continue;
        }

        auto game_system = gameplay.game_system();
        if (!game_system.is_valid()) {
            clear_players();
            continue;
        }

        auto* player_dict = game_system.player_dict();
        if (!player_dict || player_dict->count <= 0 || player_dict->count >= 1000) {
            clear_players();
            continue;
        }

        std::vector<PlayerData> current_batch;
        current_batch.reserve(player_dict->count);

        // ─── 处理角色 ───────────────────────────────────────────────────────
        for (int32_t i = 0; i < player_dict->count; ++i) {
            uintptr_t entries_addr = reinterpret_cast<uintptr_t>(player_dict->entries);
            if (!entries_addr) break;

            struct RawEntry {
                int32_t hash;
                int32_t next;
                int32_t key;
                int32_t pad;
                void*   value;
            };

            uintptr_t entry_offset = 32 + (i * 24); 
            auto entry = Memory::read<RawEntry>(entries_addr + entry_offset);

            if (entry.hash < 0 || !entry.value) continue;

            Character character(entry.value);
            Vector3 world_pos = character.position();
            
            PlayerData p_data;
            if (!camera.world_to_screen(world_pos, p_data.screen_pos, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y)) {
                continue;
            }

            p_data.is_visible = true;

            // ─── 提取骨骼 ───────────────────────────────────────────────────
            auto model = character.model();
            if (model.is_valid()) {
                auto* bones_array = model.get_bones(base_address);
                if (bones_array) {
                    int32_t bone_count = Memory::read<int32_t>(reinterpret_cast<uintptr_t>(bones_array) + 24);
                    bone_count = std::min(bone_count, 256);
                    uintptr_t bone_base = reinterpret_cast<uintptr_t>(bones_array) + 32;

                    for (const auto& conn : k_skeleton_connections) {
                        if (conn.from >= bone_count || conn.to >= bone_count) continue;

                        uintptr_t t_from = Memory::read<uintptr_t>(bone_base + conn.from * sizeof(uintptr_t));
                        uintptr_t t_to   = Memory::read<uintptr_t>(bone_base + conn.to * sizeof(uintptr_t));

                        if (!t_from || !t_to) continue;

                        Vector3 w_from = unity::get_position(reinterpret_cast<Transform*>(t_from));
                        Vector3 w_to   = unity::get_position(reinterpret_cast<Transform*>(t_to));

                        ImVec2 s_from, s_to;
                        if (camera.world_to_screen(w_from, s_from, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y) &&
                            camera.world_to_screen(w_to,   s_to,   ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y)) {
                            p_data.skeleton_lines.push_back({s_from, s_to});
                        }
                    }
                }
            }

            current_batch.push_back(std::move(p_data));
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_players = std::move(current_batch);
        }
    }
}

[[nodiscard]] std::vector<PlayerData> EspManager::players() const noexcept {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_players;
}

void EspManager::clear_players() noexcept {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_players.clear();
}

} // namespace onyx::core
