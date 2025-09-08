// src/utils/Math/Vector2.cpp
#include "Vector2.h"

// 标量乘法：Vector2 * float
Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

// 友元函数：float * Vector2（支持左乘）
Vector2 operator*(float scalar, const Vector2& vector) {
    return Vector2(scalar * vector.x, scalar * vector.y);
}

// 一元负号运算符：返回反向向量 ← 添加这个实现
Vector2 Vector2::operator-() const {
    return Vector2(-x, -y);
}

// 加法
Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

// 减法
Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

// 标量除法
Vector2 Vector2::operator/(float scalar) const {
    return Vector2(x / scalar, y / scalar);
}

// 复合加法赋值
Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// 复合减法赋值
Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// 复合标量乘法赋值
Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

// 复合标量除法赋值
Vector2& Vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

// 点积
float Vector2::dot(const Vector2& other) const {
    return x * other.x + y * other.y;
}

// 叉积（2D叉积返回标量）
float Vector2::cross(const Vector2& other) const {
    return x * other.y - y * other.x;
}

// 向量长度
float Vector2::length() const {
    return std::sqrt(x * x + y * y);
}

// 向量长度平方（优化用，避免开方）
float Vector2::lengthSq() const {
    return x * x + y * y;
}

// 返回单位向量
Vector2 Vector2::normalized() const {
    float len = length();
    return len > 0 ? Vector2(x / len, y / len) : Vector2();
}

// 原地单位化
void Vector2::normalize() {
    float len = length();
    if (len > 0) {
        x /= len;
        y /= len;
    }
}