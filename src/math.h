#ifndef MATH_H
#define MATH_H
#include <cmath>

float deg2rad(float degrees);
float rad2deg(float radians);

struct Vector2 {
    float x, y;

    Vector2(float x, float y);
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);
    float magnitude() const;
    Vector2 normalize() const;

    float dot(const Vector2& other) const;
    float distance(const Vector2& other) const;
};

struct Circle {
    Vector2 center;
    float radius;

    Circle(Vector2 center, float radius);
    bool contains(Vector2 point) const;
    bool collides(const Circle& other) const;
};

#endif
