# Critical Ops ESP Base

[![C++](https://img.shields.io/badge/C++-blue.svg)](https://en.cppreference.com/w/cpp/)
[![Android](https://img.shields.io/badge/Platform-Android-green.svg)](https://developer.android.com/)
[![Target](https://img.shields.io/badge/Version-1.60.0.f3149-red.svg)](#)
[![Arch](https://img.shields.io/badge/Arch-x86__64-orange.svg)](#)

**Onyx** is a basic base project for **Critical Ops**. It provides a simple foundation for creating esp and understanding IL2CPP structures.

---

## Target Specification

This project is specifically configured for:
* **Game Version:** 1.60.0.f3149
* **Architecture:** Android x86_64

**Note:** Using this on different versions or ARM-based architectures (armeabi-v7a/arm64-v8a) will result in crashes.

---

## Project Status

This is a learning base, not a finished product. It is currently missing many features (such as names, team identification, and proper health bars) because of the game's obfuscation. The goal is to provide a clean starting point that is easy to understand and modify.

---

## Basic Features

The project currently includes a simple Skeleton ESP setup:
* **Basic Bone Rendering**: Projects 3D world coordinates of bones onto the 2D screen.
* **Root Markers**: Simple circle markers drawn at the player's base (feet) position.
* **Overlay Rendering**: Uses a transparent ImGui background for drawing lines and markers.

![Screenshot](png/Screenshot%202026-04-15%20165955.png)

---

## Structure

The code is organized into small modules to keep it manageable:
* **SDK**: A few simple classes (Character, GameSystem) that wrap the game's memory addresses.
* **Core**: A basic loop (EspManager) that fetches player data on a background thread.
* **Render**: Simple functions using ImGui and OpenGL to draw lines on the screen.
* **Utils**: Basic memory tools for reading and writing to addresses.

---

## Tools Used

This project uses several community tools:
- **[KittyMemory](https://github.com/MJx0/KittyMemory)** by [MJx0](https://github.com/MJx0) — memory scanning and pattern finding for Android.
- **[AndKittyInjector](https://github.com/MJx0/AndKittyInjector)** by [MJx0](https://github.com/MJx0) — shared library injector for Android.
- **[Dobby](https://github.com/jmpews/Dobby)** by [jmpews](https://github.com/jmpews) — lightweight function hooking framework.

---

## Adding More Features

To expand this base, you must identify the obfuscated names and offsets yourself and update the relevant files:
1. Add new offsets in `offsets.hpp`.
2. Add matching functions in the SDK classes like `character.hpp`.
3. Use that data in the rendering loop.

It is designed to be straightforward so that developers can add their own features as they study the game's internals.

---

## Author

* `feng_xingzhe_cn_.86`
