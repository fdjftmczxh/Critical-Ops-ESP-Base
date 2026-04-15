// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 渲染系统和 ImGui 集成。
//

#pragma once
#ifndef ONYX_RENDER_RENDERER_HPP
#define ONYX_RENDER_RENDERER_HPP

#include <onyx/core/esp_manager.hpp>
#include <onyx/core/camera_system.hpp>
#include <EGL/egl.h>

namespace onyx::render {

class Renderer final {
public:
    Renderer() = default;
    ~Renderer() = default;

    // 不可复制
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    /// @brief 编排帧渲染。
    /// @param display EGL 显示器。
    /// @param surface EGL 表面。
    /// @param esp ESP 数据管理器。
    /// @param camera 相机投影系统。
    void render_frame(EGLDisplay display, EGLSurface surface, 
                      const core::EspManager& esp, 
                      core::CameraSystem& camera) noexcept;

private:
    void initialize_imgui() noexcept;
    void draw_esp(const core::EspManager& esp) noexcept;
    void apply_styling() noexcept;

    bool m_is_initialized{false};
    double m_last_time{0.0};
};

} // namespace onyx::render

#endif // ONYX_RENDER_RENDERER_HPP
