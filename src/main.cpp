// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 应用程序入口点和协调器。
//

#include <onyx/il2cpp/api.hpp>
#include <onyx/core/esp_manager.hpp>
#include <onyx/core/camera_system.hpp>
#include <onyx/render/renderer.hpp>
#include <onyx/il2cpp/collection.hpp>
#include <KittyScanner.hpp>

#include <dobby.h>
#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#define LOG_TAG "Onyx"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)

namespace {

    static onyx::core::EspManager   s_esp_manager;
    static onyx::core::CameraSystem s_camera_system;
    static onyx::render::Renderer   s_renderer;
    
    static uintptr_t s_il2cpp_base = 0;
    static bool      s_is_ready    = false;

    typedef EGLBoolean (*t_eglSwapBuffers)(EGLDisplay, EGLSurface);
    static t_eglSwapBuffers s_orig_swap = nullptr;

    EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
        if (s_is_ready) {
            s_renderer.render_frame(dpy, surface, s_esp_manager, s_camera_system);
        }
        return s_orig_swap(dpy, surface);
    }

    void* esp_worker_thread(void*) {
        LOGI("ESP 工作线程已启动。");
        s_esp_manager.update_loop(s_il2cpp_base, s_camera_system);
        return nullptr;
    }

    void* init_worker_thread(void*) {
        LOGI("初始化线程已启动。正在等待游戏...");
        sleep(5);

        if (onyx::il2cpp::init()) {
            auto il2cpp_elf = KittyScanner::ElfScanner::findElf("libil2cpp.so");
            if (il2cpp_elf.isValid()) {
                s_il2cpp_base = il2cpp_elf.base();
                s_is_ready = true;
                
                pthread_t esp_thread;
                pthread_create(&esp_thread, nullptr, esp_worker_thread, nullptr);
            }
        }

        void* egl_handle = dlopen("libEGL.so", RTLD_LAZY);
        if (egl_handle) {
            void* sym = dlsym(egl_handle, "eglSwapBuffers");
            if (sym) {
                DobbyHook(sym, 
                          reinterpret_cast<void*>(hook_eglSwapBuffers), 
                          reinterpret_cast<void**>(&s_orig_swap));
                LOGI("eglSwapBuffers 已成功挂钩 (Hooked)。");
            }
        }

        return nullptr;
    }
} // namespace

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    pthread_t init_thread;
    pthread_create(&init_thread, nullptr, init_worker_thread, nullptr);
    return JNI_VERSION_1_6;
}
