#include "math.h"

float deg2rad(float degrees) {
    return degrees * M_PI / 180.0;
}

float rad2deg(float radians) {
    return radians * 180.0 / M_PI;
}

Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const {
    return Vector2(x / scalar, y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

float Vector2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalize() const {
    return *this / magnitude();
}

float Vector2::dot(const Vector2& other) const {
    return x * other.x + y * other.y;
}

float Vector2::distance(const Vector2& other) const {
    return (*this - other).magnitude();
}

float Vector2::angleBetween(const Vector2& other) const {
    return rad2deg(std::acos(dot(other) / (magnitude() * other.magnitude())));
}

Circle::Circle(Vector2 center, float radius) : center(center), radius(radius) {}
bool Circle::contains(Vector2 point) const {
    return center.distance(point) <= radius;
}

bool Circle::collides(const Circle& other) const {
    return center.distance(other.center) <= radius + other.radius;
}
