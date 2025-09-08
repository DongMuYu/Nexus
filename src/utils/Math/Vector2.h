// src/utils/Math/Vector2.h
#pragma once
#include <cmath>

class Vector2 {
public:
    float x, y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    
    // 基本运算运算符重载
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    
    // 一元负号运算符
    Vector2 operator-() const;
    
    // 复合赋值运算符重载
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);
    
    // 点积、叉积
    float dot(const Vector2& other) const;
    float cross(const Vector2& other) const;
    
    // 向量操作
    float length() const;
    float lengthSq() const;
    Vector2 normalized() const;
    void normalize();

    // 友元函数：支持 float * Vector2 形式的乘法
    friend Vector2 operator*(float scalar, const Vector2& vector);
};