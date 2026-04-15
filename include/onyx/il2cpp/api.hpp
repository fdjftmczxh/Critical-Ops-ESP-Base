// 版权所有 (c) 2026 Onyx 作者
// 作者: feng_xingzhe_cn_.86
// SPDX-License-Identifier: MIT
//
// 使用官方引擎头文件的 Android 现代 IL2CPP API 包装器。
//

#pragma once
#ifndef ONYX_IL2CPP_API_HPP
#define ONYX_IL2CPP_API_HPP

#include <cstdint>
#include <string>

// ─── 官方 IL2CPP 头文件 (来自 Deps) ──────────────────────────────────────────
#include <il2cpp-api-types.h>
#include <il2cpp-object-internals.h>
#include <il2cpp-class-internals.h>

#include "Vector3.h"
#include "Matrix4x4.h"

namespace onyx::il2cpp {

// ─── 原生结构体别名 ──────────────────────────────────────────────────────────
using Vector3   = ::Vector3;
using Matrix4x4 = ::Matrix4x4;

// ─── 增强代码可读性的类型别名 ────────────────────────────────────────────────
using Object    = Il2CppObject;
using Array     = Il2CppArray;
using Class     = Il2CppClass;
using Domain    = Il2CppDomain;
using Assembly  = Il2CppAssembly;
using Image     = Il2CppImage;
using Method    = MethodInfo;
using Field     = FieldInfo;
using String    = Il2CppString;
using Transform = void; // 不透明 Unity 类型
using Component = void; // 不透明 Unity 类型
using Camera    = void; // 不透明 Unity 类型

// ─── API 函数指针 ───────────────────────────────────────────────────────────

namespace api {
    extern Domain* (*domain_get)();
    extern Assembly** (*domain_get_assemblies)(const Domain* domain, size_t* size);
    extern Image* (*assembly_get_image)(const Assembly* assembly);
    extern const char* (*image_get_name)(const Image* image);
    extern size_t (*image_get_class_count)(const Image* image);
    extern Class* (*image_get_class)(const Image* image, size_t index);
    extern const char* (*class_get_name)(Class* klass);
    extern const char* (*class_get_namespace)(Class* klass);
    extern const Method** (*class_get_methods)(Class* klass, void** iter); // 引擎中的 MethodInfo**
    extern Field* (*class_get_fields)(Class* klass, void** iter);
    extern const char* (*method_get_name)(const Method* method);
    extern uint32_t (*method_get_param_count)(const Method* method);
    extern void* (*thread_attach)(Domain* domain); // 返回 Il2CppThread*
    extern Class* (*class_from_name)(const Image* image, const char* namespaze, const char* name);
    extern const Method* (*class_get_method_from_name)(Class* klass, const char* name, int args_count);
    extern uint32_t (*array_length)(Array* array);
    extern void* (*object_unbox)(Object* obj);
    extern String* (*string_new)(const char* str);
    extern Class* (*object_get_class)(Object* obj);
    extern Object* (*runtime_invoke)(const Method* method, void* obj, void** params, Object** exc);
} // namespace api

// ─── Unity 引擎辅助函数 ──────────────────────────────────────────────────────

namespace unity {
    extern Transform* (*get_transform)(Component* component);
    extern Vector3    (*get_position)(Transform* transform);
    extern Camera*    (*get_camera_main)();
    extern Camera*    (*get_camera_current)();
    
    // 矩阵访问 (需要 MethodInfo 以确保稳定性)
    extern Matrix4x4 (*get_world_to_camera_matrix)(Camera* camera, const Method* method);
    extern Matrix4x4 (*get_projection_matrix)(Camera* camera, const Method* method);
    
    extern const Method* mi_get_world_to_camera_matrix;
    extern const Method* mi_get_projection_matrix;
    
    extern Vector3 (*world_to_screen_point)(Camera* camera, Vector3 world_pos);
} // namespace unity

/// @brief 通过扫描 libil2cpp.so 初始化 IL2CPP API。
/// @returns 如果所有基本函数都已解析，则返回 true。
[[nodiscard]] bool init() noexcept;

} // namespace onyx::il2cpp

#endif // ONYX_IL2CPP_API_HPP
