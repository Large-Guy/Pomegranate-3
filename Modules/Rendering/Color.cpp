#include "Color.h"

Color::Color() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 0;
}

Color::Color(float v) {
    this->r = v;
    this->g = v;
    this->b = v;
    this->a = 1.0f;
}

Color::Color(float r, float g, float b, float a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color Color::operator+(const Color& other) const {
    return {this->r + other.r, this->g + other.g, this->b + other.b, this->a + other.a};
}

Color Color::operator-(const Color& other) const {
    return {this->r - other.r, this->g - other.g, this->b - other.b, this->a - other.a};
}

Color Color::operator*(const Color& other) const {
    return {this->r * other.r, this->g * other.g, this->b * other.b, this->a * other.a};
}

Color Color::operator/(const Color& other) const {
    return {this->r / other.r, this->g / other.g, this->b / other.b, this->a / other.a};
}

Color Color::operator*(float scalar) const {
    return *this * Color(scalar, scalar, scalar, scalar);
}

Color Color::operator/(float scalar) const {
    return *this / Color(scalar, scalar, scalar, scalar);
}

Color& Color::operator*=(float scalar) {
    return *this *= Color(scalar, scalar, scalar, scalar);
}

Color& Color::operator/=(float scalar) {
    return *this /= Color(scalar, scalar, scalar, scalar);
}

Color& Color::operator+=(const Color& other) {
    this->r += other.r;
    this->g += other.g;
    this->b += other.b;
    this->a += other.a;
    return *this;
}

Color& Color::operator-=(const Color& other) {
    this->r -= other.r;
    this->g -= other.g;
    this->b -= other.b;
    this->a -= other.a;
    return *this;
}

Color& Color::operator*=(const Color& other) {
    this->r *= other.r;
    this->g *= other.g;
    this->b *= other.b;
    this->a *= other.a;
    return *this;
}

Color& Color::operator/=(const Color& other) {
    this->r /= other.r;
    this->g /= other.g;
    this->b /= other.b;
    this->a /= other.a;
    return *this;
}

Color Color::mix(const Color& other, float amount) const {
    return (*this - other) * amount + *this;
}


