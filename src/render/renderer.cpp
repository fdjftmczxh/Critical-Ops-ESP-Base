// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 渲染系统的实现。
//

#include <onyx/render/renderer.hpp>
#include <imgui.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>
#include <ctime>
#include <GLES3/gl3.h>
#include <algorithm>

namespace onyx::render {

void Renderer::render_frame(EGLDisplay display, EGLSurface surface, 
                           const core::EspManager& esp, 
                           core::CameraSystem& camera) noexcept {
    if (!m_is_initialized) {
        initialize_imgui();
    }

    EGLint w, h;
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));

    struct timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    double now = static_cast<double>(ts.tv_sec) + ts.tv_nsec / 1e9;
    io.DeltaTime = m_last_time > 0.0 ? static_cast<float>(now - m_last_time) : 1.0f / 60.0f;
    m_last_time = now;

    camera.update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    draw_esp(esp);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::initialize_imgui() noexcept {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    apply_styling();
    m_is_initialized = true;
}

void Renderer::draw_esp(const core::EspManager& esp) noexcept {
    auto draw_list = ImGui::GetBackgroundDrawList();
    auto players = esp.players();

    const ImU32 skeleton_color = IM_COL32(0, 255, 127, 200);
    const ImU32 head_color = IM_COL32(255, 255, 255, 255);

    for (const auto& player : players) {
        if (!player.is_visible) continue;

        // ─── 骨骼 ────────────────────────────────────────────────────────────
        for (const auto& line : player.skeleton_lines) {
            draw_list->AddLine(line.p1, line.p2, skeleton_color, 2.5f);
        }

        // ─── 头部标记 ─────────────────────────────────────────────────────────
        draw_list->AddCircleFilled(player.screen_pos, 4.0f, head_color, 16);
        draw_list->AddCircle(player.screen_pos, 6.0f, skeleton_color, 16, 1.0f);
    }
}

void Renderer::apply_styling() noexcept {
    auto& style = ImGui::GetStyle();
    
    style.WindowRounding = 8.0f;
    style.ChildRounding  = 6.0f;
    style.FrameRounding  = 6.0f;
    style.PopupRounding  = 6.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabRounding   = 6.0f;
    style.TabRounding    = 6.0f;

    style.AntiAliasedLines = true;
    style.AntiAliasedFill  = true;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.05f, 0.07f, 0.94f);
    colors[ImGuiCol_Header]                 = ImVec4(0.12f, 0.11f, 0.14f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.25f, 0.24f, 0.27f, 1.00f);
}

} // namespace onyx::render
