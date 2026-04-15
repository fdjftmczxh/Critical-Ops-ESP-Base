#pragma once

#include <cmath>
#include <string>
#include <algorithm>

// Forward declaration of Vector3 (assumed to be in your project)
struct Vector3;

struct Quaternion {
    union {
        struct { float x, y, z, w; };
        float data[4];
    };

    static constexpr float kEpsilon = 0.000001f;

    // Constructors
    inline constexpr Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) {}
    inline constexpr Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Static Accessor
    static inline Quaternion identity() { return Quaternion(0.f, 0.f, 0.f, 1.f); }

    // Set components
    inline void Set(float newX, float newY, float newZ, float newW) {
        x = newX; y = newY; z = newZ; w = newW;
    }

    // Access by index
    inline float operator[](int index) const {
        return data[index];
    }

    inline float& operator[](int index) {
        return data[index];
    }

    // --- Math Operations ---

    inline static float Dot(const Quaternion& a, const Quaternion& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    inline static float Angle(const Quaternion& a, const Quaternion& b) {
        float dot = std::min(std::abs(Dot(a, b)), 1.0f);
        return (dot > 1.0f - kEpsilon) ? 0.0f : std::acos(dot) * 2.0f * 57.29578f; // Rad2Deg
    }

    inline static Quaternion Normalize(const Quaternion& q) {
        float mag = std::sqrt(Dot(q, q));
        if (mag < 1e-06f) return identity();
        return Quaternion(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
    }

    inline void Normalize() {
        *this = Normalize(*this);
    }

    inline Quaternion normalized() const {
        return Normalize(*this);
    }

    // --- Operators ---

    // Combines rotations (lhs * rhs)
    inline Quaternion operator*(const Quaternion& rhs) const {
        return Quaternion(
                w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
                w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
                w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
        );
    }

    // Rotates a Vector3 by this Quaternion
    inline Vector3 operator*(const Vector3& point) const;

    inline bool operator==(const Quaternion& rhs) const {
        return Dot(*this, rhs) > 1.0f - kEpsilon;
    }

    inline bool operator!=(const Quaternion& rhs) const {
        return !(*this == rhs);
    }

    // --- Euler Angle Conversions ---
    // Note: Standard Unity Euler order is ZXY
    static inline Quaternion Euler(float x, float y, float z);
    static inline Quaternion Euler(const Vector3& euler);
    inline Vector3 eulerAngles() const;

    // String representation
    inline std::string str() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
    }
};

// Implementation of Vector3 rotation (needs Vector3 definition)
#include "Vector3.h"

inline Vector3 Quaternion::operator*(const Vector3& point) const {
    float tx = x * 2.0f;
    float ty = y * 2.0f;
    float tz = z * 2.0f;
    float txx = x * tx;
    float tyy = y * ty;
    float tzz = z * tz;
    float txy = x * ty;
    float txz = x * tz;
    float tyz = y * tz;
    float twx = w * tx;
    float twy = w * ty;
    float twz = w * tz;

    Vector3 res;
    res.x = (1.0f - (tyy + tzz)) * point.x + (txy - twz) * point.y + (txz + twy) * point.z;
    res.y = (txy + twz) * point.x + (1.0f - (txx + tzz)) * point.y + (tyz - twx) * point.z;
    res.z = (txz - twy) * point.x + (tyz + twx) * point.y + (1.0f - (txx + tyy)) * point.z;
    return res;
}

// Basic Euler to Quaternion implementation
inline Quaternion Quaternion::Euler(float x, float y, float z) {
    float deg2rad = 0.0174532925f;
    float c1 = std::cos(y * deg2rad / 2);
    float s1 = std::sin(y * deg2rad / 2);
    float c2 = std::cos(z * deg2rad / 2);
    float s2 = std::sin(z * deg2rad / 2);
    float c3 = std::cos(x * deg2rad / 2);
    float s3 = std::sin(x * deg2rad / 2);

    return Quaternion(
            s1 * s2 * c3 + c1 * c2 * s3,
            s1 * c2 * c3 + c1 * s2 * s3,
            c1 * s2 * c3 - s1 * c2 * s3,
            c1 * c2 * c3 - s1 * s2 * s3
    );
}

inline Quaternion Quaternion::Euler(const Vector3& euler) {
    return Euler(euler.x, euler.y, euler.z);
}