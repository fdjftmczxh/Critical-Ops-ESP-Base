cmake_minimum_required(VERSION 3.22.1)

project("imgui_hook")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/Deps/imgui)

# 1. Define paths to dependencies
set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps)
set(KITTYMEMORY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps/KittyMemory)
set(DOBBY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps/Dobby)
set(IL2CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps/Il2cpp)

# 2. Correctly find all source files in KittyMemoryEx and its subdirectories
file(GLOB_RECURSE KITTYMEMORY_SRC
    "${KITTYMEMORY_DIR}/*.cpp"
    "${KITTYMEMORY_DIR}/*.c"
)

# 3. Add include directories
include_directories(
    ${KITTYMEMORY_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${DOBBY_DIR}
    ${IL2CPP_DIR}
    ${DEPS_DIR}/Structures
    include
    src
)

# ImGui Source Files
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_android.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# 4. Set prebuilt library paths using ${ANDROID_ABI}
set(KEYSTONE_LIB ${KITTYMEMORY_DIR}/Deps/Keystone/libs-android/${ANDROID_ABI}/libkeystone.a)
set(DOBBY_LIB ${DOBBY_DIR}/${ANDROID_ABI}/libdobby.a)

# 5. Create the shared library
add_library(${CMAKE_PROJECT_NAME} SHARED
    src/main.cpp
    src/il2cpp/api.cpp
    src/core/camera_system.cpp
    src/core/esp_manager.cpp
    src/render/renderer.cpp
    ${IMGUI_SOURCES}
    ${KITTYMEMORY_SRC}
)

# 6. Link dependencies
target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log
    GLESv3
    EGL
    ${DOBBY_LIB}
    ${KEYSTONE_LIB}
)

# Optional: Add compiler flags
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -fvisibility=hidden)
