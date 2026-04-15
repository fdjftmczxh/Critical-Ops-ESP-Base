// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 使用官方引擎头文件实现的 IL2CPP API 解析系统。
//

#include <onyx/il2cpp/api.hpp>
#include <KittyScanner.hpp>
#include <dlfcn.h>
#include <android/log.h>

#define LOG_TAG "Onyx"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace onyx::il2cpp {

namespace api {
    Domain* (*domain_get)() = nullptr;
    Assembly** (*domain_get_assemblies)(const Domain*, size_t*) = nullptr;
    Image* (*assembly_get_image)(const Assembly*) = nullptr;
    const char* (*image_get_name)(const Image*) = nullptr;
    size_t (*image_get_class_count)(const Image*) = nullptr;
    Class* (*image_get_class)(const Image*, size_t) = nullptr;
    const char* (*class_get_name)(Class*) = nullptr;
    const char* (*class_get_namespace)(Class*) = nullptr;
    const Method** (*class_get_methods)(Class*, void**) = nullptr;
    Field* (*class_get_fields)(Class*, void**) = nullptr;
    const char* (*method_get_name)(const Method*) = nullptr;
    uint32_t (*method_get_param_count)(const Method*) = nullptr;
    void* (*thread_attach)(Domain*) = nullptr;
    Class* (*class_from_name)(const Image*, const char*, const char*) = nullptr;
    const Method* (*class_get_method_from_name)(Class*, const char*, int) = nullptr;
    uint32_t (*array_length)(Array*) = nullptr;
    void* (*object_unbox)(Object*) = nullptr;
    String* (*string_new)(const char*) = nullptr;
    Class* (*object_get_class)(Object*) = nullptr;
    Object* (*runtime_invoke)(const Method*, void*, void**, Object**) = nullptr;
} // namespace api

namespace unity {
    Transform* (*get_transform)(Component*) = nullptr;
    Vector3    (*get_position)(Transform*) = nullptr;
    Camera*    (*get_camera_main)() = nullptr;
    Camera*    (*get_camera_current)() = nullptr;
    Matrix4x4 (*get_world_to_camera_matrix)(Camera*, const Method*) = nullptr;
    Matrix4x4 (*get_projection_matrix)(Camera*, const Method*) = nullptr;
    const Method* mi_get_world_to_camera_matrix = nullptr;
    const Method* mi_get_projection_matrix = nullptr;
    Vector3 (*world_to_screen_point)(Camera*, Vector3) = nullptr;
} // namespace unity

namespace {
    struct Signature {
        const char* name;
        const char* pattern;
    };

    const Signature k_signatures[] = {
        {"il2cpp_runtime_invoke",             "41 57 41 56 41 54 53 50 48 89 CB 49 89 D6 49 89 F7 49 89 FC 48 85 C9"},
        {"il2cpp_domain_get",                 "48 8B 05 ? ? ? ? 48 85 C0 74 ? C3 53"},
        {"il2cpp_domain_get_assemblies",      "53 48 89 F3 E8 ? ? ? ? ? ? ? 48 8B 40"},
        {"il2cpp_thread_attach",              "41 57 41 56 53 48 89 FB 48 8B 05 ? ? ? ? ? ? E8"},
        {"il2cpp_assembly_get_image",         "E9 ? ? ? ? E9 ? ? ? ? E9 ? ? ? ? 8A 87"},
        {"il2cpp_image_get_name",             "E9 ? ? ? ? E9 ? ? ? ? E9 ? ? ? ? 50 E8 ? ? ? ? 89 C0"},
        {"il2cpp_image_get_class_count",      "50 E8 ? ? ? ? 89 C0"},
        {"il2cpp_image_get_class",            "50 E8 ? ? ? ? 48 89 C7 58 E9 ? ? ? ? ? 41 57"},
        {"il2cpp_class_get_name",             "48 8B 47 ? C3 ? 41 57 41 56 41 54 53 48 83 EC ? 48 89 F3 49 89 FE 48 85 F6"},
        {"il2cpp_class_get_namespace",        "48 8B 47 ? C3 ? 48 8B 47 ? C3 ? 41 57"},
        {"il2cpp_class_get_methods",          "48 85 F6 74 ? 41 56 53 50 48 89 F3 49 89 FE ? ? ? 48 85 C0 74 ? 48 83 C0 ? 41 0F B7 8E ? ? ? ? C1 E1 ? 49 03 8E ? ? ? ? 48 39 C8 72 ? EB ? 31 C0 C3 4C 89 F7 E8 ? ? ? ? 66 41 83 BE ? ? ? ? 00 74 ? 49 8B 86 ? ? ? ? ? ? ? ? ? ? EB ? 31 C0 48 83 C4 ? 5B 41 5E C3 ? 41 56 53 50 48 89 FB"},
        {"il2cpp_class_get_fields",           "48 85 F6 74 ? 41 56 53 50 48 89 F3 49 89 FE ? ? ? 48 85 C0 74 ? 48 83 C0 ? 41 0F B7 8E ? ? ? ? C1 E1 ? 49 03 8E ? ? ? ? 48 39 C8 72 ? EB ? 31 C0 C3 4C 89 F7 E8 ? ? ? ? 66 41 83 BE ? ? ? ? 00 74 ? 49 8B 86 ? ? ? ? ? ? ? EB"},
        {"il2cpp_method_get_name",            "48 8B 47 ? C3 ? 48 8B 47 ? C3 ? 41 57"},
        {"il2cpp_method_get_param_count",     "0F B6 47 ? C3 ? 0F B6 47"},
        {"il2cpp_class_from_name",            "55 41 57 41 56 41 55 41 54 53 48 83 EC ? 49 89 D6 49 89 F7 48 89 FB 48 8B 77 ? 48 85 F6 0F 85"},
        {"il2cpp_class_get_method_from_name", "31 C9 45 31 C0 E9 ? ? ? ? 45 31 C0 E9 ? ? ? ? 55 41 57 41 56 41 55 41 54 53 48 83 EC"},
        {"il2cpp_array_length",               "8B 47 ? C3 55 41 57 41 56 41 55 41 54 53 48 83 EC ? 49 89 D6 49 89 F7 48 89 FB"},
        {"il2cpp_object_unbox",               "48 8D 47 ? C3 ? 55 41 57 41 56 53"},
        {"il2cpp_string_new",                 "53 48 89 FB E8 ? ? ? ? 48 89 DF 89 C6 5B E9 ? ? ? ? 41 57 41 56 53"},
        {"il2cpp_object_get_class",           "E9 ? ? ? ? E9 ? ? ? ? E9 ? ? ? ? 50 E8 ? ? ? ? 59"}
    };

    /// @brief 使用 KittyScanner 在内存中安全查找特征码。
    [[nodiscard]] uintptr_t safe_scan(const KittyScanner::ElfScanner& elf, const char* pattern) noexcept {
        auto segments = elf.segments();
        for (const auto& seg : segments) {
            if (!seg.readable) continue;
            uintptr_t found = KittyScanner::findIdaPatternFirst(seg.startAddress, seg.endAddress, pattern);
            if (found) return found;
        }
        return 0;
    }

    /// @brief 解析指定的 Unity 方法。
    [[nodiscard]] const Method* resolve_unity_method(Image* img, const char* ns, const char* klass_name, const char* meth_name, int args) noexcept {
        Class* klass = api::class_from_name(img, ns, klass_name);
        if (!klass) return nullptr;
        return api::class_get_method_from_name(klass, meth_name, args);
    }
} // namespace

bool init() noexcept {
    LOGI("正在初始化 Onyx IL2CPP API (官方头文件模式)...");

    auto il2cpp_elf = KittyScanner::ElfScanner::findElf("libil2cpp.so");
    if (!il2cpp_elf.isValid()) {
        LOGE("未找到 libil2cpp.so");
        return false;
    }

    auto resolve = [&](const char* name, void** ptr) {
        for (const auto& sig : k_signatures) {
            if (strcmp(sig.name, name) == 0) {
                *ptr = reinterpret_cast<void*>(safe_scan(il2cpp_elf, sig.pattern));
                break;
            }
        }
        if (!*ptr) {
            *ptr = dlsym(RTLD_DEFAULT, name);
        }
        if (!*ptr) LOGE("无法解析: %s", name);
        return *ptr != nullptr;
    };

    bool success = true;
    success &= resolve("il2cpp_domain_get",             (void**)&api::domain_get);
    success &= resolve("il2cpp_domain_get_assemblies",  (void**)&api::domain_get_assemblies);
    success &= resolve("il2cpp_assembly_get_image",     (void**)&api::assembly_get_image);
    success &= resolve("il2cpp_image_get_name",         (void**)&api::image_get_name);
    success &= resolve("il2cpp_image_get_class_count",  (void**)&api::image_get_class_count);
    success &= resolve("il2cpp_image_get_class",        (void**)&api::image_get_class);
    success &= resolve("il2cpp_class_get_name",         (void**)&api::class_get_name);
    success &= resolve("il2cpp_class_get_namespace",    (void**)&api::class_get_namespace);
    success &= resolve("il2cpp_class_get_methods",      (void**)&api::class_get_methods);
    success &= resolve("il2cpp_class_get_fields",       (void**)&api::class_get_fields);
    success &= resolve("il2cpp_method_get_name",        (void**)&api::method_get_name);
    success &= resolve("il2cpp_method_get_param_count", (void**)&api::method_get_param_count);
    success &= resolve("il2cpp_thread_attach",          (void**)&api::thread_attach);
    success &= resolve("il2cpp_class_from_name",        (void**)&api::class_from_name);
    success &= resolve("il2cpp_class_get_method_from_name", (void**)&api::class_get_method_from_name);
    success &= resolve("il2cpp_array_length",           (void**)&api::array_length);
    success &= resolve("il2cpp_object_unbox",           (void**)&api::object_unbox);
    success &= resolve("il2cpp_string_new",             (void**)&api::string_new);
    success &= resolve("il2cpp_object_get_class",       (void**)&api::object_get_class);
    success &= resolve("il2cpp_runtime_invoke",         (void**)&api::runtime_invoke);

    if (!success) return false;

    size_t assembly_count = 0;
    Assembly** assemblies = api::domain_get_assemblies(api::domain_get(), &assembly_count);
    Image* core_module = nullptr;

    for (size_t i = 0; i < assembly_count; i++) {
        Image* img = api::assembly_get_image(assemblies[i]);
        std::string name = api::image_get_name(img);
        if (name == "UnityEngine.CoreModule.dll" || name == "UnityEngine.dll") {
            core_module = img;
            break;
        }
    }

    if (!core_module) return false;

    auto resolve_unity = [&](const char* ns, const char* klass, const char* meth, int args, void** ptr) {
        const Method* mi = resolve_unity_method(core_module, ns, klass, meth, args);
        if (mi) *ptr = reinterpret_cast<void*>(mi->methodPointer);
        return *ptr != nullptr;
    };

    success &= resolve_unity("UnityEngine", "Component", "get_transform", 0, (void**)&unity::get_transform);
    success &= resolve_unity("UnityEngine", "Transform", "get_position", 0, (void**)&unity::get_position);
    success &= resolve_unity("UnityEngine", "Camera",    "get_main",     0, (void**)&unity::get_camera_main);
    success &= resolve_unity("UnityEngine", "Camera",    "get_current",  0, (void**)&unity::get_camera_current);
    
    // 矩阵访问 (需要 MethodInfo 以确保稳定性)
    const Method* mi_wtc = resolve_unity_method(core_module, "UnityEngine", "Camera", "get_worldToCameraMatrix", 0);
    if (mi_wtc) {
        unity::get_world_to_camera_matrix = (Matrix4x4 (*)(Camera*, const Method*))mi_wtc->methodPointer;
        unity::mi_get_world_to_camera_matrix = mi_wtc;
    }

    const Method* mi_proj = resolve_unity_method(core_module, "UnityEngine", "Camera", "get_projectionMatrix", 0);
    if (mi_proj) {
        unity::get_projection_matrix = (Matrix4x4 (*)(Camera*, const Method*))mi_proj->methodPointer;
        unity::mi_get_projection_matrix = mi_proj;
    }

    resolve_unity("UnityEngine", "Camera", "WorldToScreenPoint", 1, (void**)&unity::world_to_screen_point);

    LOGI("Onyx API 初始化完成");
    return true;
}

} // namespace onyx::il2cpp
