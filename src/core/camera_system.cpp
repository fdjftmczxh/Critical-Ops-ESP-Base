// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 相机坐标转换系统的实现。
//

#include <onyx/core/camera_system.hpp>

namespace onyx::core {

void CameraSystem::update() noexcept {
    using namespace il2cpp;

    Camera* cam = unity::get_camera_main ? unity::get_camera_main() : nullptr;
    if (!cam && unity::get_camera_current) {
        cam = unity::get_camera_current();
    }

    if (!cam) return;

    if (unity::get_world_to_camera_matrix && unity::mi_get_world_to_camera_matrix &&
        unity::get_projection_matrix && unity::mi_get_projection_matrix) {
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_view_matrix = unity::get_world_to_camera_matrix(cam, unity::mi_get_world_to_camera_matrix);
        m_proj_matrix = unity::get_projection_matrix(cam, unity::mi_get_projection_matrix);
        m_is_valid = true;
    }
}

[[nodiscard]] bool CameraSystem::world_to_screen(const Vector3& world_pos, 
                                               ImVec2& out_screen, 
                                               float screen_width, 
                                               float screen_height) const noexcept {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_is_valid) return false;

    // Vector4 转换
    Vector4 view_pos = m_view_matrix * Vector4(world_pos.x, world_pos.y, world_pos.z, 1.0f);
    Vector4 clip_pos = m_proj_matrix * view_pos;

    if (clip_pos.w < 0.1f) return false;

    // NDC 除法
    Vector3 ndc(clip_pos.x / clip_pos.w, clip_pos.y / clip_pos.w, clip_pos.z / clip_pos.w);

    // 屏幕映射 (ImGui 使用左上角原点)
    out_screen.x = (screen_width / 2.0f) + (ndc.x * screen_width / 2.0f);
    out_screen.y = (screen_height / 2.0f) - (ndc.y * screen_height / 2.0f);

    return true;
}

} // namespace onyx::core
