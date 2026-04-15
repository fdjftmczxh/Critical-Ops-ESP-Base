// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 相机同步和坐标转换系统。
//

#pragma once
#ifndef ONYX_CORE_CAMERA_SYSTEM_HPP
#define ONYX_CORE_CAMERA_SYSTEM_HPP

#include <onyx/il2cpp/api.hpp>
#include <mutex>
#include <imgui.h>

namespace onyx::core {

class CameraSystem final {
public:
    CameraSystem() = default;
    ~CameraSystem() = default;

    // 不可复制
    CameraSystem(const CameraSystem&) = delete;
    CameraSystem& operator=(const CameraSystem&) = delete;

    /// @brief 从 Unity 引擎同步矩阵。
    /// 应从渲染线程调用。
    void update() noexcept;

    /// @brief 使用缓存矩阵将世界坐标转换为屏幕空间。
    /// @param world_pos 游戏世界中的 3D 位置。
    /// @param out_screen 输出屏幕坐标。
    /// @param screen_width 当前视口宽度。
    /// @param screen_height 当前视口高度。
    /// @returns 如果点在可见视锥体内，则返回 true。
    [[nodiscard]] bool world_to_screen(const Vector3& world_pos, 
                                       ImVec2& out_screen, 
                                       float screen_width, 
                                       float screen_height) const noexcept;

private:
    mutable std::mutex m_mutex;
    Matrix4x4 m_view_matrix;
    Matrix4x4 m_proj_matrix;
    bool      m_is_valid{false};
};

} // namespace onyx::core

#endif // ONYX_CORE_CAMERA_SYSTEM_HPP
