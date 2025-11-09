#ifndef POMEGRANATE_COLOR_H
#define POMEGRANATE_COLOR_H


struct Color {
    float r, g, b, a;

    Color();
    
    explicit Color(float v);
    
    Color(float r, float g, float b, float a = 1.0f);

    //Operators
    Color operator+(const Color& other) const;
    Color operator-(const Color& other) const;
    Color operator*(const Color& other) const;
    Color operator/(const Color& other) const;
    Color operator*(float scalar) const;
    Color operator/(float scalar) const;

    Color& operator+=(const Color& other);
    Color& operator-=(const Color& other);
    Color& operator*=(const Color& other);
    Color& operator/=(const Color& other);
    Color& operator*=(float scalar);
    Color& operator/=(float scalar);

    Color mix(const Color& other, float amount) const;
};


#endif //POMEGRANATE_COLOR_H