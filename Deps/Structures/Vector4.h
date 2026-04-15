#pragma once

#include <cmath>
#include <string>
#include <algorithm>
#include <limits>

// Forward declarations to allow implicit conversions
struct Vector2;
struct Vector3;

struct Vector4 {
    float x, y, z, w;

    static constexpr float kEpsilon = 0.00001f;

    // Constructors
    inline constexpr Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    inline constexpr Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    inline constexpr Vector4(float x, float y, float z) : x(x), y(y), z(z), w(0.f) {}
    inline constexpr Vector4(float x, float y) : x(x), y(y), z(0.f), w(0.f) {}

    // Set components
    inline void Set(float newX, float newY, float newZ, float newW) {
        x = newX; y = newY; z = newZ; w = newW;
    }

    // Static Methods
    inline static Vector4 Lerp(Vector4 a, Vector4 b, float t) {
        t = std::clamp(t, 0.f, 1.f);
        return Vector4(
                a.x + (b.x - a.x) * t,
                a.y + (b.y - a.y) * t,
                a.z + (b.z - a.z) * t,
                a.w + (b.w - a.w) * t
        );
    }

    inline static Vector4 LerpUnclamped(Vector4 a, Vector4 b, float t) {
        return Vector4(
                a.x + (b.x - a.x) * t,
                a.y + (b.y - a.y) * t,
                a.z + (b.z - a.z) * t,
                a.w + (b.w - a.w) * t
        );
    }

    inline static Vector4 MoveTowards(Vector4 current, Vector4 target, float maxDistanceDelta) {
        float to_x = target.x - current.x;
        float to_y = target.y - current.y;
        float to_z = target.z - current.z;
        float to_w = target.w - current.w;

        float sqdist = to_x * to_x + to_y * to_y + to_z * to_z + to_w * to_w;

        if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta))
            return target;

        float dist = std::sqrt(sqdist);

        return Vector4(
                current.x + to_x / dist * maxDistanceDelta,
                current.y + to_y / dist * maxDistanceDelta,
                current.z + to_z / dist * maxDistanceDelta,
                current.w + to_w / dist * maxDistanceDelta
        );
    }

    inline static Vector4 Scale(Vector4 a, Vector4 b) {
        return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline void Scale(Vector4 scale) {
        x *= scale.x; y *= scale.y; z *= scale.z; w *= scale.w;
    }

    inline static float Dot(Vector4 a, Vector4 b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    inline static Vector4 Normalize(Vector4 a) {
        float mag = a.magnitude();
        if (mag > kEpsilon) return a / mag;
        return zero();
    }

    inline void Normalize() {
        float mag = magnitude();
        if (mag > kEpsilon) *this = *this / mag;
        else *this = zero();
    }

    // Properties
    inline float magnitude() const { return std::sqrt(Dot(*this, *this)); }
    inline float sqrMagnitude() const { return Dot(*this, *this); }
    inline Vector4 normalized() const { return Vector4::Normalize(*this); }

    inline static float Distance(Vector4 a, Vector4 b) {
        return (a - b).magnitude();
    }

    inline static Vector4 Project(Vector4 a, Vector4 b) {
        return b * (Dot(a, b) / Dot(b, b));
    }

    inline static Vector4 Min(Vector4 lhs, Vector4 rhs) {
        return Vector4(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w));
    }

    inline static Vector4 Max(Vector4 lhs, Vector4 rhs) {
        return Vector4(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w));
    }

    // Operators
    inline float operator[](int index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: return 0.f;
        }
    }

    inline Vector4 operator+(const Vector4& b) const { return Vector4(x + b.x, y + b.y, z + b.z, w + b.w); }
    inline Vector4 operator-(const Vector4& b) const { return Vector4(x - b.x, y - b.y, z - b.z, w - b.w); }
    inline Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
    inline Vector4 operator*(float d) const { return Vector4(x * d, y * d, z * d, w * d); }
    inline Vector4 operator/(float d) const { return Vector4(x / d, y / d, z / d, w / d); }

    inline bool operator==(const Vector4& rhs) const {
        Vector4 diff = *this - rhs;
        return diff.sqrMagnitude() < (kEpsilon * kEpsilon);
    }
    inline bool operator!=(const Vector4& rhs) const { return !(*this == rhs); }

    // Static Accessors
    static inline Vector4 zero() { return Vector4(0.f, 0.f, 0.f, 0.f); }
    static inline Vector4 one() { return Vector4(1.f, 1.f, 1.f, 1.f); }
    static inline Vector4 positiveInfinity() {
        float inf = std::numeric_limits<float>::infinity();
        return Vector4(inf, inf, inf, inf);
    }
    static inline Vector4 negativeInfinity() {
        float inf = -std::numeric_limits<float>::infinity();
        return Vector4(inf, inf, inf, inf);
    }

    // String Conversion (Obfuscation Removed)
    inline std::string str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
    }
};

inline Vector4 operator*(float d, const Vector4& a) {
    return Vector4(a.x * d, a.y * d, a.z * d, a.w * d);
}